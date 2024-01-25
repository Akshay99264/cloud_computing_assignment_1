#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h>
#include <linux/sched/signal.h> // Needed for for_each_process

MODULE_DESCRIPTION("Kernel Module to print child process id's and state");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
static int PID; // Default parameter value

// Define module parameters
module_param(PID, int, S_IRUGO);
char buffer[256];
//This fuction provide the status of the process based on __state value of task_struct.
char *getStatus(long status)
{
    switch (status) {
        case 0:
            return "TASK_RUNNING";
        case 1:
            return "TASK_INTERRUPTIBLE";
        case 2:
            return "TASK_UNINTERRUPTIBLE";
        case 4:
            return "__TASK_STOPPED";
        case 8:
            return "__TASK_TRACED";
        default:
        {
            sprintf(buffer, "Unknown Type:%ld\n", status);
            return buffer;
        }
    }
}

int getChild(void)
{

    struct pid *pid_struct;
    struct task_struct *child_process, *task;
    struct list_head *list;

    // Checking valid PID or not.
    if (PID == -1)
    {
        printk(KERN_ALERT "Invalid input\n");
        return 0;
    }

    // Search for the task with given PID.
    pid_struct = find_get_pid(PID);
    task = pid_task(pid_struct, PIDTYPE_PID);

    // If no task found print error message.
    if (task == NULL)
    {
        printk(KERN_ALERT "No process with pid : %d\n",PID);
        return 0;
    }
    // Print the PID.
    printk(KERN_INFO "Parent process ID : %d\n", task->pid);

    // Travers over all the child process and print their PID and state.
    list_for_each(list, &task->children)
    {
        child_process = list_entry(list, struct task_struct, sibling);
        // st = getStatus(task->__state);
        printk("Process ID of child : %d, State of process : %s\n", child_process->pid, getStatus(child_process->__state));
    }
    return 0;
}

// init function
static int my_init(void)
{
    getChild();
    return 0;
}

// exit function
static void my_exit(void)
{
    return;
}

module_init(my_init);
module_exit(my_exit);
