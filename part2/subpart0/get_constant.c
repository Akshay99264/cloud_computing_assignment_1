#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
 
#define FIRST_MINOR 0
#define MINOR_CNT 1
#define QUERY_GET_CONSTANT _IOR('q', 1, int)
static dev_t dev;
static struct cdev c_dev;
static struct class *cl; 
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

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    int constant = 28;
 
    switch (cmd)
    {
        case QUERY_GET_CONSTANT:
            if (copy_to_user((int *)arg, &constant, sizeof(int)))
            {
                return -EACCES;
            }
            break;
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
    .unlocked_ioctl = my_ioctl
};
 
static int __init query_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0) // function allocates a range of character device numbers. 
    {
        return ret;
    }
 
    cdev_init(&c_dev, &query_fops); // This function initializes a struct cdev structure
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) // This function adds the character device to the system. 
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char"))) // This function creates a device class
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "getConstant"))) // This function creates a device and registers it with the kernel.
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
MODULE_DESCRIPTION("Query ioctl() to get constant");
