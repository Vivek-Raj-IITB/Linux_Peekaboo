#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <linux/mmzone.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>

MODULE_DESCRIPTION("My kernel module 5");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

int pid=1;
module_param(pid,int, S_IRUSR|S_IWUSR);
int *new_memory;
static int find_virtual_and_physical(void){
        struct task_struct *task_list;
        unsigned long virtualSpace=0, physicalSpace=0;
        for_each_process(task_list) {
                if(task_list->pid==pid){
                        struct mm_struct *mm;
                        struct vm_area_struct *vma;
                        mm=task_list->mm;
                        vma=mm->mmap;
                        while(vma){
                                virtualSpace=virtualSpace+vma->vm_end-vma->vm_start;
                                vma=vma->vm_next;
                        }
                        physicalSpace = get_mm_rss(mm) << PAGE_SIZE;
                        printk(KERN_INFO "PID:[%d]  Virtual Address Space: %lu and Physical Address Space: 0%lu\n",pid,virtualSpace, physicalSpace);
                        // printk("Yes,Linux implement lazy memory allocation we can see the virtual address space and physical address space are not equal that means all the requirement of the given process is not loaded to main memory hence 'Lazy memory allocation'\n");
                        return 0;
                        
                }
        }
        printk(KERN_INFO "There is no task with pid %d found\n", pid);
        return 0;
}
static void lazy_memory_allocation(void){
        char *my_buffer;
        size_t buffer_size = 1024 * 1024;
        my_buffer = vmalloc(buffer_size);
        printk("before allocating to allocate memory\n");
        find_virtual_and_physical();
        if (!my_buffer) {
                printk(KERN_ERR "Failed to allocate memory\n");
                
        }

        memset(my_buffer, 0, buffer_size);
        printk("after allocating to allocate memory\n");
        find_virtual_and_physical();

        /* Use my_buffer for something */

        kfree(my_buffer);
        

}
static int hello_world(void){
        printk(KERN_INFO "Module 5 loaded\n");
        rcu_read_lock();
        find_virtual_and_physical();
        printk("Demonstration of lazy memory allocation \n");
        lazy_memory_allocation();
        return 0;
}
static void hello_exit(void){
        printk(KERN_INFO "Module 5 unloaded\n");
}

module_init(hello_world);
module_exit(hello_exit);
