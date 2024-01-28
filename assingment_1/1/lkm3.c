#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/pid.h>
#include <linux/mm.h>
#include <asm/io.h> // contains phys_to_page(page)
#include <asm/pgtable.h>

MODULE_DESCRIPTION("Kernel Module to print the physical address");
MODULE_AUTHOR("Akshay Patidar");
MODULE_LICENSE("GPL");

unsigned int PID = 0;
unsigned long virtual_address = 0;
module_param(PID, int, S_IRUGO);
module_param(virtual_address, ulong, S_IRUGO);

unsigned long get_physical_address(struct mm_struct *mm, ulong virtualAddress)
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
    if (!(pte = pte_offset_map(pmd, virtualAddress)))
        return 0;
    if (!(page = pte_page(*pte)))
        return 0;
    physical_address = page_to_phys(page); // Converts a struct page pointer to the corresponding physical address of the page in memory. 
    return physical_address;
}
static int func_init(void)
{
    struct task_struct *task; // represents the task or process control block,
    struct pid *pID; // struct pid structure is used to uniquely identify a process and is associated with a specific process in the system.
    struct mm_struct *mm; // represents the memory management information for a process.
    unsigned long physical_address;
    printk(KERN_INFO "Module lkm3 inserted successfully\n");
    pID = find_get_pid(PID); // function that is used to find and reference a process descriptor (struct pid) associated with a given process ID (PID). 
    if (!pID)
    {
        printk(KERN_ALERT "Invalid PID\n");
        return -EINVAL; // or another appropriate error code
    }

    task = pid_task(pID, PIDTYPE_PID); // function t used to obtain a pointer to the task structure (struct task_struct) associated with a given process identifier (PID).
    if (!task)
    {
        printk(KERN_ALERT "Invalid task for PID\n");
        return -EINVAL; // or another appropriate error code
    }

    mm = task->mm; 
    physical_address = get_physical_address(mm, virtual_address);
    printk(KERN_INFO "Process ID: %d, Virtual address: %li, Physical address: %ld\n", PID, virtual_address, physical_address);
    return 0;
}

static void func_exit(void)
{
    printk(KERN_INFO "Module lkm3 exited successfully\n");
    return;
}

module_init(func_init);
module_exit(func_exit);