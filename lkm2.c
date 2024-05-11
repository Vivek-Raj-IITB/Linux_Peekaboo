
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h> 
#include <linux/sched/task.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>     
#include <linux/mm.h> 


MODULE_DESCRIPTION("My kernel module 2");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

static int modinit(void){
    struct task_struct *tsk;
    struct mm_struct *mm;
    static unsigned long memory=0;
    static int task_pid=-1;
    for_each_process(tsk){
        mm = get_task_mm(tsk);
        if (mm) {
            struct vm_area_struct *vma = tsk->mm->mmap;
            unsigned long heap = 0;
            while(vma){
                if (vma->vm_flags & VM_GROWSDOWN){
                    heap += vma->vm_end - vma->vm_start;
                }
                vma = vma->vm_next;
            }
            if(heap > memory){
                memory = heap;
                task_pid = tsk->pid;
            }
        }
        else continue;
    }
    printk(KERN_INFO "Process with highest heap size is %d with heap size %lu bytes\n",task_pid,memory);

    return 0;
}

static int hello_world(void){
        printk(KERN_INFO "Module 2 loaded\n");
        modinit();
        return 0;
}

static void hello_exit(void){
        printk(KERN_INFO "Module 2 unloaded\n");
}

module_init(hello_world);
module_exit(hello_exit);