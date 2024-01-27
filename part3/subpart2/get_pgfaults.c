#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/mm.h>     /* Needed for vmstat.h */
#include <linux/vmstat.h> /* Needed for all_vm_events */
#include <linux/proc_fs.h>

static unsigned long *counter_buf;
static int procfile_read(struct seq_file *m, void *v)
{

    all_vm_events(counter_buf);
    seq_printf(m, "Page Faults: %lu\n", counter_buf[PGFAULT]);
    return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, procfile_read, NULL); // function is a helper function used to simplify the implementation of the open operation for files that are read sequentially (like those under the /proc filesystem
}
static struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
};

static struct proc_dir_entry *entry; // hold a reference to a directory entry in the /proc filesystem,

static int __init hello_func_init(void)
{
    counter_buf = kmalloc(NR_VM_ZONE_STAT_ITEMS * sizeof(unsigned long) + sizeof(struct vm_event_state) + 100,
                          GFP_KERNEL); // extra 100 is "just in case" :)
    if (counter_buf == NULL)
    {
        printk("Crud! kmalloc failed.\n");
    }
    entry = proc_create("get_pgfaults", 0, NULL, &proc_fops); // function is used for creating entries in the /proc filesystem
    if (!entry)
    {
        printk(KERN_ERR "Failed to create /proc/get_pgfaults\n");
        return -ENOMEM; // Error code indicating memory allocation failure
    }
    return 0;
}

static void __exit hello_func_exit(void)
{
    proc_remove(entry);
    if (counter_buf != NULL)
    {
        kfree(counter_buf);
        counter_buf = NULL;
    }
}

module_init(hello_func_init);
module_exit(hello_func_exit);
MODULE_LICENSE("GPL");