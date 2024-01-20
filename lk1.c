#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/sched/signal.h>
#define KERN_INFO
MODULE_LICENSE("GPL");

struct cfs_rq {
        struct load_weight load;
        unsigned int nr_running, h_nr_running;
};

void printList(void){
    struct task_struct * tsk;
    for_each_process(tsk){
        if(tsk->__state)
            continue;
        printk("pid: %d\n", tsk->pid);
    }
}

static int __init my_init(void)
{
   printList();
    return 0;
}

static void __exit my_exit(void)
{
    return;
}

module_init(my_init);
module_exit(my_exit);
