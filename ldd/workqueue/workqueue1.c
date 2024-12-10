#include <linux/module.h>
#include<linux/delay.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

static struct workqueue_struct *my_wq; // Custom workqueue
static struct delayed_work my_delayed_work; // Delayed work item

// Delayed work handler function
static void delayed_work_handler(struct work_struct *work) {
    printk(KERN_INFO "Workqueue: Executing delayed work handler\n");
    // Simulate some processing
    msleep(1000);
    printk(KERN_INFO "Workqueue: Delayed work completed\n");
}

// Module initialization
static int __init delayed_workqueue_example_init(void) {
    printk(KERN_INFO "Initializing Delayed Workqueue Example Module\n");

    // Create a custom workqueue
    my_wq = alloc_workqueue("my_wq", WQ_UNBOUND, 0);
    if (!my_wq) {
        printk(KERN_ERR "Failed to create workqueue\n");
        return -ENOMEM;
    }

    // Initialize delayed work and queue it after 5 seconds
    INIT_DELAYED_WORK(&my_delayed_work, delayed_work_handler);
    queue_delayed_work(my_wq, &my_delayed_work, msecs_to_jiffies(5000));

    return 0;
}

// Module cleanup
static void __exit delayed_workqueue_example_exit(void) {
    printk(KERN_INFO "Cleaning up Delayed Workqueue Example Module\n");

    // Cancel any pending delayed work and destroy the workqueue
    cancel_delayed_work_sync(&my_delayed_work);
    destroy_workqueue(my_wq);
}

module_init(delayed_workqueue_example_init);
module_exit(delayed_workqueue_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Delayed Workqueue Example");
