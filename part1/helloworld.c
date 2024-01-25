#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_DESCRIPTION("My kernel module");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

static int hello_init(void)
{
        printk("Hello World\n");
        return 0;
}

static void bye_exit(void)
{
        printk("Module Unloaded\n");
}

module_init(hello_init);
module_exit(bye_exit);