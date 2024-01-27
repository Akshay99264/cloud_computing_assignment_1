#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


static int print_hello_world(struct seq_file *m, void *v) {
    seq_printf(m, "Hello World!\n");
    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, print_hello_world, NULL); // function is a helper function used to simplify the implementation of the open operation for files that are read sequentially (like those under the /proc filesystem
}
static struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
};

static struct proc_dir_entry *entry; // hold a reference to a directory entry in the /proc filesystem, 

static int __init hello_func_init(void) {
    entry = proc_create("hello_procfs", 0, NULL, &proc_fops); // function is used for creating entries in the /proc filesystem
    if (!entry) {
        printk(KERN_ERR "Failed to create /proc/hello_procfs\n");
        return -ENOMEM; // Error code indicating memory allocation failure
    }
    return 0;
}

static void __exit hello_func_exit(void) {
    proc_remove(entry);
}

module_init(hello_func_init);
module_exit(hello_func_exit);
MODULE_LICENSE("GPL");