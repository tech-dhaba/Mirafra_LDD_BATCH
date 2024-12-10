#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/timer.h>

// Define SoftIRQ
static struct tasklet_struct my_tasklet;

// Define a timer for triggering the SoftIRQ
static struct timer_list my_timer;

// Tasklet handler function
static void tasklet_handler(unsigned long data)
{
    pr_info("SoftIRQ executed in tasklet context with data: %ld\n", data);
}

// Timer callback function to simulate hardware event
static void timer_callback(struct timer_list *t)
{
    pr_info("Timer callback triggered\n");

    // Schedule the tasklet to simulate SoftIRQ
    tasklet_schedule(&my_tasklet);

    // Restart the timer for periodic simulation
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000)); // 1 second interval
}

// Module initialization
static int __init softirq_driver_init(void)
{
    pr_info("Initializing SoftIRQ driver\n");

    // Initialize the tasklet
    tasklet_init(&my_tasklet, tasklet_handler, 0);

    // Initialize the timer
    timer_setup(&my_timer, timer_callback, 0);

    // Start the timer
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

    pr_info("SoftIRQ driver loaded\n");
    return 0;
}

// Module cleanup
static void __exit softirq_driver_exit(void)
{
    // Cleanup tasklet
    tasklet_kill(&my_tasklet);

    // Cleanup timer
    del_timer_sync(&my_timer);

    pr_info("SoftIRQ driver unloaded\n");
}

module_init(softirq_driver_init);
module_exit(softirq_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("A SoftIRQ driver example using tasklets");

