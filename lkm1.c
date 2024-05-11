#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("My kernel module 1");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

// char * get_task_state(long state){

//     if(state == 1)return "TASK_RUNNING";
//     return "NOT_RUNNING";
   
// }

static int find_running_tasks(void){
    struct task_struct *task_list;
    for_each_process(task_list) {
        if(task_list->state==TASK_RUNNING||task_list->state == TASK_UNINTERRUPTIBLE ||
            task_list->state == TASK_INTERRUPTIBLE)pr_info("Process: %s\t PID:[%d]\t State:%s\n", task_list->comm, task_list->pid,
        task_list->state); 
    }
    return 0;
}

static int hello_world(void){
        printk(KERN_INFO "Module 1 loaded\n");
        find_running_tasks();
        return 0;
}

static void hello_exit(void){
        printk(KERN_INFO "Module 1 unloaded\n");
}

module_init(hello_world);
module_exit(hello_exit);