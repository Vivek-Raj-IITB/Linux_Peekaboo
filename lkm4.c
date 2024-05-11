#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/io.h>

MODULE_DESCRIPTION("My kernel module 4");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

int pid=0;
unsigned long virtualAddress=0;
module_param(pid,int, S_IRUSR|S_IWUSR);
module_param(virtualAddress, ulong, S_IRUSR | S_IWUSR);

static int find_virtual_to_physical(void){
    struct task_struct *task_list;
    for_each_process(task_list) {
        if(task_list->pid==pid){
                struct mm_struct *mm;
                struct vm_area_struct *vma;
                unsigned long physicalAddress;
                mm=task_list->mm;
                vma = find_vma(mm,virtualAddress);
                if(!vma){
                        printk(KERN_INFO "Virtual address: 0x%lx of process pid: %d is not mapped with pyhsical address\n", virtualAddress, pid);
                        return 0;
                }
                physicalAddress = virt_to_phys(vma->vm_start)+(virtualAddress-vma->vm_start);
                printk(KERN_INFO "PID:[%d]  Virtual Address: 0x%lx Corresponding Physical Address: 0x%lx\n",pid,virtualAddress, physicalAddress);
                return 0;
        }

    }
    printk(KERN_INFO "There is no task with pid %d found\n", pid);

    return 0;
}
static int hello_world(void){
        printk(KERN_INFO "Module 4 loaded\n");
        find_virtual_to_physical();
        return 0;
}

static void hello_exit(void){
        printk(KERN_INFO "Module 4 unloaded\n");
}

module_init(hello_world);
module_exit(hello_exit);
