#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("My kernel module 3" );
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");


static int test_tasks_init(void){
    struct task_struct *task_list;
    void* stack_pointer;
    for_each_process(task_list) {
        if(task_list->pid==1)stack_pointer=task_list->stack;
    }
    pr_info("PID:[1] Stack Pointer: %pK\n",stack_pointer);
    return 0;
}

static int hello_world(void){
        printk(KERN_INFO "Module 3 loaded\n");
        test_tasks_init();
        return 0;
}

static void hello_exit(void){
        printk(KERN_INFO "Module 3 unloaded\n");
}

module_init(hello_world);
module_exit(hello_exit);