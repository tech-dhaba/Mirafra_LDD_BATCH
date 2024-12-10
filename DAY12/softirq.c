#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("A simple Linux kernel module to demonstrate softirqs");

static struct tasklet_struct my_tasklet;

declare_softirq(my_softirq, softirq_handler);

// Softirq handler function
static void softirq_handler(struct softirq_action *action)
{
    printk(KERN_INFO "Softirq: Executing softirq handler\n");
}

// Tasklet function to trigger softirq
static void tasklet_function(unsigned long data)
{
    printk(KERN_INFO "Tasklet: Scheduling softirq\n");
    raise_softirq(MY_SOFTIRQ);
}

// Module init function
static int __init softirq_example_init(void)
{
    printk(KERN_INFO "Softirq example module loaded\n");
    // Register softirq
    open_softirq(MY_SOFTIRQ, softirq_handler);
}
