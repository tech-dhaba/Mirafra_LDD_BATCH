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
    data++;
    pr_info("SoftIRQ executed in tasklet context with data: %ld\n", data);
}

// Timer callback function to simulate hardware event
static int count = 0;
static void timer_callback(struct timer_list *t)
{
    pr_info("Timer callback triggered %d\n", count);
    count++;

    // Schedule the tasklet to simulate SoftIRQ
    tasklet_schedule(&my_tasklet);

    // Restart the timer for periodic simulation
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000)); // 1 second interval
}

static int count1 = 0;
static int normal_function(unsigned long count1)
{
    int i;
    for (i = 0; i < 10; i++)  // Limit the iterations to 10
    {
        pr_info("inside the normal function %ld\n", count1);
        count1++;
    }
    return 0;
}

// Module initialization
static int __init softirq_driver_init(void)
{
    pr_info("Initializing SoftIRQ driver\n");

    // Initialize the tasklet
    tasklet_init(&my_tasklet, tasklet_handler, 0);
    normal_function(count1);

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
MODULE_AUTHOR("Rajini");
MODULE_DESCRIPTION("A SoftIRQ driver example using tasklets");
