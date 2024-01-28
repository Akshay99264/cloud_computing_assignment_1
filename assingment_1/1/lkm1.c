#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h>
#include <linux/sched/signal.h> //Needed for for_each_process

MODULE_DESCRIPTION("Kernel Module to print process ID of all running or runnable processes");
MODULE_AUTHOR("Akshay Patidar");
MODULE_LICENSE("GPL");

void getPIDs(void)
{
    struct task_struct *task;
    // for_each_process is a MACRO iterate over all the processes
    for_each_process(task)
    {
        // Task_running: A process that is running or is ready to run in the ready queue run-queue and actually participates in process scheduling.
        // task->__state will be 0 for the process in running or runnable state.
        if (task->__state)
            continue;
        // print the PID of those process whose __state value is 0, means process is either running or in runnable state.
        printk(KERN_INFO "PID: %d\n", task->pid);
    }
}

static int my_init(void)
{
    printk(KERN_INFO "Module lkm1 inserted successfully\n");
    getPIDs();
    return 0;
}

static void my_exit(void)
{
    printk(KERN_INFO "Module lkm1 exited successfully\n");
    return;
}

module_init(my_init);
module_exit(my_exit);
