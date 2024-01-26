#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h> // contains phys_to_page(page)
#include <asm/pgtable.h>

#define FIRST_MINOR 0 
#define MINOR_CNT 1
#define QUERY_GET_PHYSICAL_ADDR _IOR('q', 1, unsigned long) // case to get physical address
#define QUERY_CHANGE_VALUE _IOW('q', 2, chang_para) // case to change the value give physical address
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

// struct to handle the case to get physical address given virtual address and pid.
typedef struct
{
    int pid;
    unsigned long virtual_address;
    unsigned long physical_address;   
} get_phys;

// struct to handle the case to modify the content given physical address.
typedef struct
{
    unsigned long physical_address;
    char value;
} chang_para;

static int my_open(struct inode *i, struct file *f)
{
    printk("ioctl driver get opend\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk("ioctl driver get closed\n");
    return 0;
}

unsigned long get_physical_address(struct mm_struct *mm, unsigned long virtualAddress)
{
    pgd_t *pgd; // The Page Global Directory is the top-level page table entry in the x86_64 architecture. It points to the start of the P4D (Page Level 4 Directory).
    p4d_t *p4d; // The Page Level 4 Directory is the second level in the x86_64 page table hierarchy. It is used to map large ranges of virtual memory.
    pud_t *pud; // The Page Upper Directory is the third level in the x86_64 page table hierarchy.
    pmd_t *pmd; // The Page Middle Directory is the fourth level in the x86_64 page table hierarchy.
    pte_t *pte; // The Page Table Entry is the bottom-level page table entry in the x86_64 architecture. It points to the physical page frame that corresponds to a specific virtual address.
    unsigned long physical_address;
    // unsigned long physicalAddress;
    struct page *page;
    pgd = pgd_offset(mm, virtualAddress); 
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return 0;
    p4d = p4d_offset(pgd, virtualAddress);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
        return 0;
    pud = pud_offset(p4d, virtualAddress);
    if (pud_none(*pud) || pud_bad(*pud))
        return 0;
    pmd = pmd_offset(pud, virtualAddress);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        return 0;
    if (!(pte = pte_offset_kernel(pmd, virtualAddress)))
        return 0;
    if (!(page = pte_page(*pte)))
        return 0;
    physical_address = page_to_phys(page) + offset_in_page(virtualAddress);
    return physical_address;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    
    switch (cmd)
    {
    case QUERY_GET_PHYSICAL_ADDR:
    {
        get_phys temp;
        if (copy_from_user(&temp, (get_phys *)arg, sizeof(get_phys)))
        {
            return -EFAULT;
        }
        int PID = temp.pid;
        unsigned long virtual_address = temp.virtual_address;
        struct task_struct *task; // represents the task or process control block,
        struct pid *pID;          // struct pid structure is used to uniquely identify a process and is associated with a specific process in the system.
        struct mm_struct *mm;     // represents the memory management information for a process.
        unsigned long physical_address;
        pID = find_get_pid(PID); // function that is used to find and reference a process descriptor (struct pid) associated with a given process ID (PID).
        if (!pID)
        {
            printk("Invalid PID\n");
            return -EINVAL; // or another appropriate error code
        }

        task = pid_task(pID, PIDTYPE_PID); // function t used to obtain a pointer to the task structure (struct task_struct) associated with a given process identifier (PID).
        if (!task)
        {
            printk("Invalid task for PID\n");
            return -EINVAL; // or another appropriate error code
        }

        mm = task->mm;
       
        physical_address = get_physical_address(mm, virtual_address);
        temp.physical_address = physical_address;
        // if (copy_to_user((int *)arg, &temp, sizeof(int)))
        // {
        //     return -EACCES;
        // }
        if (copy_to_user((get_phys *)arg, &temp, sizeof(get_phys)))
        {
            return -EFAULT;
        }
        break;
    }
    case QUERY_CHANGE_VALUE:
    {
        chang_para temp;
        if (copy_from_user(&temp, (chang_para *)arg, sizeof(chang_para)))
        {
            return -EFAULT;
        }
        *(char *)phys_to_virt(temp.physical_address) = temp.value; // modify the value given physical address.
        printk("Modification Successfull\n");
        break;
    }
    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations query_fops =
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .unlocked_ioctl = my_ioctl};

static int __init query_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;

    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "v2p_file")) < 0) // function allocates a range of character device numbers.
    {
        return ret;
    }

    cdev_init(&c_dev, &query_fops); // This function initializes a struct cdev structure

    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) // This function adds the character device to the system.
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "change_v2p_p2V"))) // This function creates a device class
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "v2p_p2v_device"))) // This function creates a device and registers it with the kernel.
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit query_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(query_ioctl_init);
module_exit(query_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshay Patidar");
MODULE_DESCRIPTION("Driver to perform virtual to physical address and Modify content given physical address");
