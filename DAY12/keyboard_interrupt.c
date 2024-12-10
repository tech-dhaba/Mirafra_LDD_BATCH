#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#define KEYBOARD_IRQ 1  // Example IRQ number for keyboard, adjust as needed

// Interrupt handler function
static irqreturn_t vajid_keyboard_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Vajid Keyboard IRQ handler triggered\n");
    // Add your keyboard handling logic here

    return IRQ_HANDLED;
}

// Module initialization function
static int __init vajid_init(void)
{
    int ret;

    // Request IRQ for the keyboard
    ret = request_irq(KEYBOARD_IRQ, vajid_keyboard_irq_handler, IRQF_SHARED, 
                      "vajid_keyboard", NULL);
    if (ret) {
        printk(KERN_ERR "Failed to register Vajid keyboard IRQ\n");
        return ret;
    }

    printk(KERN_INFO "Vajid keyboard IRQ handler registered\n");
    return 0;
}

// Module exit function
static void __exit vajid_exit(void)
{
    free_irq(KEYBOARD_IRQ, NULL);
    printk(KERN_INFO "Vajid keyboard IRQ handler unregistered\n");
}

module_init(vajid_init);
module_exit(vajid_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module for Vajid keyboard IRQ handling");
