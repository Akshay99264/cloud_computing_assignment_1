#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h>
#include <linux/sched/signal.h> //Needed for for_each_process

MODULE_LICENSE("GPL");
static int PID ;  // Default parameter value

// Define module parameters
module_param(PID, int, S_IRUGO);

int getChild(void){

    if(PID == -1){
        printk(KERN_ALERT "No input entered!\n");
        return 0;
    }

    struct pid *pid_struct;
    struct task_struct *parent;
    pid_struct = find_get_pid(PID);
    parent = pid_task(pid_struct, PIDTYPE_PID);

    if(parent == NULL){
        printk(KERN_ALERT "No process found!\n");
        return 0;
    }

    struct task_struct *task;
    struct list_head *list;

    printk(KERN_INFO "%d\n", parent->pid);

    list_for_each(list, &parent->children) {
        task = list_entry(list, struct task_struct, sibling);
        printk(KERN_INFO "%d  %d\n", task->pid, task->__state);
    }
   return 0;
}
static int my_init(void)
{
    getChild();
    return 0;
}

static void my_exit(void)
{
    return;
}

module_init(my_init);
module_exit(my_exit);
