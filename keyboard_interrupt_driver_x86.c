#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

// IRQ number for the keyboard
#define KEYBOARD_IRQ 1

// Interrupt handler function
static irqreturn_t my_keyboard_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "My: Keyboard interrupt received!\n");
    return IRQ_HANDLED; // Successfully handled the interrupt
}

// Module initialization
static int __init my_keyboard_interrupt_init(void)
{
    int result;
    printk(KERN_INFO "My: Initializing the Keyboard Interrupt Driver\n");

    // Request the IRQ line for keyboard interrupt
    result = request_irq(KEYBOARD_IRQ, my_keyboard_irq_handler, IRQF_SHARED,
                         "my_keyboard_interrupt_driver", (void *)my_keyboard_irq_handler);
    if (result) {
        printk(KERN_ERR "My: Failed to request IRQ %d, error: %d\n", KEYBOARD_IRQ, result);
        return result;
    }

    printk(KERN_INFO "My: Keyboard Interrupt Driver successfully loaded\n");
    return 0;
}

// Module cleanup function
static void __exit my_keyboard_interrupt_exit(void)
{
    free_irq(KEYBOARD_IRQ, (void *)my_keyboard_irq_handler); // Release the IRQ
    printk(KERN_INFO "My: Keyboard Interrupt Driver unloaded\n");
}

// Register module initialization and exit functions
module_init(my_keyboard_interrupt_init);
module_exit(my_keyboard_interrupt_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("my");
MODULE_DESCRIPTION("My Keyboard Interrupt Driver");

