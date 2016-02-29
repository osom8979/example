/**
 * @file   m02.c
 * @brief  Print process list.
 * @author zer0
 * @date   2015-03-06
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

//#include <asm/thread_info.h>
#include <linux/kthread.h>

int __init init_hello(void)
{
    printk(KERN_ALERT "Driver m02 init.\n");
    return 0;
}

void __exit exit_hello(void)
{
    struct task_struct * task;

    for_each_process(task) {
        printk("%s[%d]\n", task->comm, task->pid);
    }
    printk(KERN_ALERT "Driver m02 exit.\n");
}

module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");

