/**
 * @file   m03.c
 * @brief  Create kernel trhead.
 * @author zer0
 * @date   2015-03-06
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define _INIT_MESSAGE "Driver m03 init.\n"
#define _EXIT_MESSAGE "Driver m03 exit.\n"

struct task_struct * g_th_id = NULL;

static int test_kthread_entry(void * arg)
{
    int count = 0;

    printk(KERN_ALERT "@ %s() : called\n", __FUNCTION__);

    while (!kthread_should_stop()) {
        printk(KERN_ALERT "@ %s() : loop %d\n", __FUNCTION__, count);
        ssleep(1); 
        count++;
    }

    printk(KERN_ALERT "Virtual Runtime: %llu", g_th_id->se.vruntime);
    printk(KERN_ALERT "@ %s() : kthread_should_stop() called. Bye.\n", __FUNCTION__);

    return 0;
}

int __init init_test_module(void)
{
    printk(KERN_ALERT _INIT_MESSAGE);

    if (g_th_id == NULL) { 
        g_th_id = (struct task_struct *) kthread_run(test_kthread_entry
                , NULL, "kthread_test");
    }

    return 0;
}

void __exit exit_test_module(void)
{
    if (g_th_id) {
        kthread_stop(g_th_id);
        g_th_id = NULL;
    }

    printk(KERN_ALERT _EXIT_MESSAGE);
}

module_init(init_test_module);
module_exit(exit_test_module);

MODULE_LICENSE("GPL");

