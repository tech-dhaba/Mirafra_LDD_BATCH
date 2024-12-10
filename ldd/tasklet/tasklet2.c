#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#define DEVICE_NAME "advanced_tasklet_device"
#define MAX_BUFFER_SIZE 1024
void status_tasklet_handler(unsigned long data);
void data_tasklet_handler(unsigned long data);
irqreturn_t simulated_interrupt_handler(int irq, void *dev_id);

// Shared data buffers
static char *status_buffer;
static char *data_buffer;
static int status_len = 0;
static int data_len = 0;

// Spinlock for synchronization
static spinlock_t buffer_lock;

// Tasklet structures
static struct tasklet_struct status_tasklet;
static struct tasklet_struct data_tasklet;

// Tasklet handlers
void status_tasklet_handler(unsigned long data) {
    unsigned long flags;
    
    // Protect the shared buffer using a spinlock
    spin_lock_irqsave(&buffer_lock, flags);
    if (status_len > 0) {
        printk(KERN_INFO "Status Tasklet: Processing status: %.*s\n", status_len, status_buffer);
        memset(status_buffer, 0, MAX_BUFFER_SIZE);
        status_len = 0;
    } else {
        printk(KERN_INFO "Status Tasklet: No status to process\n");
    }
    spin_unlock_irqrestore(&buffer_lock, flags);
}

void data_tasklet_handler(unsigned long data) {
    unsigned long flags;

    // Protect the shared buffer using a spinlock
    spin_lock_irqsave(&buffer_lock, flags);
    if (data_len > 0) {
        printk(KERN_INFO "Data Tasklet: Processing data: %.*s\n", data_len, data_buffer);
        memset(data_buffer, 0, MAX_BUFFER_SIZE);
        data_len = 0;
    } else {
        printk(KERN_INFO "Data Tasklet: No data to process\n");
    }
    spin_unlock_irqrestore(&buffer_lock, flags);
}

// Simulated interrupt handler
irqreturn_t simulated_interrupt_handler(int irq, void *dev_id) {
    unsigned long flags;

    printk(KERN_INFO "Interrupt occurred: Scheduling tasklets\n");

    // Simulate filling buffers with status and data
    spin_lock_irqsave(&buffer_lock, flags);
    strncpy(status_buffer, "Device Status: OK", MAX_BUFFER_SIZE);
    status_len = strlen(status_buffer);

    strncpy(data_buffer, "Packet Data: Hello, World!", MAX_BUFFER_SIZE);
    data_len = strlen(data_buffer);
    spin_unlock_irqrestore(&buffer_lock, flags);

    // Schedule the tasklets
    tasklet_schedule(&status_tasklet);
    tasklet_schedule(&data_tasklet);

    return IRQ_HANDLED;
}

// Module Initialization
static int __init advanced_tasklet_init(void) {
    printk(KERN_INFO "Initializing Advanced Tasklet Module\n");

    // Allocate memory for buffers
    status_buffer = kzalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
    if (!status_buffer) {
        printk(KERN_ERR "Failed to allocate memory for status buffer\n");
        return -ENOMEM;
    }

    data_buffer = kzalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
    if (!data_buffer) {
        printk(KERN_ERR "Failed to allocate memory for data buffer\n");
        kfree(status_buffer);
        return -ENOMEM;
    }

    // Initialize spinlock
    spin_lock_init(&buffer_lock);

    // Initialize tasklets
    tasklet_init(&status_tasklet, status_tasklet_handler, 0);
    tasklet_init(&data_tasklet, data_tasklet_handler, 0);

    // Simulate requesting an interrupt (in real-world, you'd use request_irq)
    printk(KERN_INFO "Simulating an interrupt\n");
    simulated_interrupt_handler(0, NULL);

    return 0;
}

// Module Cleanup
static void __exit advanced_tasklet_exit(void) {
    printk(KERN_INFO "Cleaning up Advanced Tasklet Module\n");

    // Kill the tasklets
    tasklet_kill(&status_tasklet);
    tasklet_kill(&data_tasklet);

    // Free allocated memory
    kfree(status_buffer);
    kfree(data_buffer);
}

module_init(advanced_tasklet_init);
module_exit(advanced_tasklet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Advanced Tasklet Example Module");
