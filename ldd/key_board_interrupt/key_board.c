/* file name : key board interrupt
 * author : yaswanth reddy 
 * description: a simple keyboard interface code 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

/*  IRQ number for the keyboard  */
#define KEYBOARD_IRQ 1

/*  Interrupt handler function */
static irqreturn_t irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Keyboard interrupt received!\n");
    return IRQ_HANDLED; /* Successfully handled the interrupt  */
}

/*  Module initialization  */
static int __init interrupt_init(void)
{
    int result;
    printk(KERN_INFO "Initializing the Keyboard Interrupt Driver\n");

    /* Request the IRQ line for keyboard interrupt */
    result = request_irq(KEYBOARD_IRQ, irq_handler, IRQF_SHARED,
                         "keyboard_interrupt_driver", (void *)irq_handler);
    if (result) {
        printk(KERN_ERR "Failed to request IRQ %d, error: %d\n", KEYBOARD_IRQ, result);
        return result;
    }

    printk(KERN_INFO "Keyboard Interrupt Driver successfully loaded\n");
    return 0;
}

/*  Module cleanup function   */
static void __exit interrupt_exit(void)
{
    free_irq(KEYBOARD_IRQ, (void *)irq_handler); // Release the IRQ
    printk(KERN_INFO "Keyboard Interrupt Driver unloaded\n");
}

/*  Register module initialization and exit functions */
module_init(interrupt_init);
module_exit(interrupt_exit);

/*  Module metadata  */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("yaswanth reddy");
MODULE_DESCRIPTION("yaswanth  Keyboard Interrupt Driver");
