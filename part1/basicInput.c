#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Sample kernel module with command-line arguments");

static int PID ;  // Default parameter value

// Define module parameters
module_param(PID, int, S_IRUGO);  // charp: character pointer, S_IRUGO: read-only permission

static int __init my_init(void) {
    printk(KERN_INFO "PID is %d\n", PID);
    return 0;
}

static void __exit my_exit(void) {
    printk(KERN_INFO "Module removed\n");
}

module_init(my_init);
module_exit(my_exit);
