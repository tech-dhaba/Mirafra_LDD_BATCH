#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/jiffies.h>  // For jiffies and time handling

#define GPIO_BUTTON 531  // Button GPIO pin
#define GPIO_LED 532  // LED GPIO pin

static unsigned int irq_number;
static irqreturn_t button_irq_handler(int irq, void *dev_id);

// Variable for debouncing
static unsigned long last_jiffies = 0;
#define DEBOUNCE_DELAY_MS 200  // Debounce delay in milliseconds

// Driver initialization
static int __init gpio_driver_init(void)
{
    int result;

    // Request GPIO pins for button and LED
    if ((result = gpio_request(GPIO_BUTTON, "GPIO_BUTTON"))) {
        printk(KERN_ERR "Failed to request GPIO_BUTTON sush\n");
        return result;
    }
    if ((result = gpio_request(GPIO_LED, "GPIO_LED"))) {
        printk(KERN_ERR "Failed to request GPIO_LED sush\n");
        gpio_free(GPIO_BUTTON);
        return result;
    }

    // Set GPIO directions
    gpio_direction_input(GPIO_BUTTON);  // Button as input
    gpio_direction_output(GPIO_LED, 0); // LED as output, initially off

    // Map GPIO pin to IRQ line
    irq_number = gpio_to_irq(GPIO_BUTTON);
    if (irq_number < 0) {
        printk(KERN_ERR "Failed to map GPIO to IRQ\n");
        gpio_free(GPIO_BUTTON);
        gpio_free(GPIO_LED);
        return irq_number;
    }

    // Request interrupt and set up handler
    result = request_irq(irq_number, button_irq_handler, IRQF_TRIGGER_FALLING, "gpio_button_irq", NULL);
    if (result) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpio_free(GPIO_BUTTON);
        gpio_free(GPIO_LED);
        return result;
    }

    printk(KERN_INFO "GPIO Interrupt Driver Loaded with Debouncing\n");
    return 0;
}

// Driver cleanup
static void __exit gpio_driver_exit(void)
{
    free_irq(irq_number, NULL);  // Free interrupt
    gpio_free(GPIO_BUTTON);      // Free GPIO pins
    gpio_free(GPIO_LED);
    printk(KERN_INFO "GPIO Interrupt Driver Unloaded\n");
}

// The interrupt handler with debouncing
static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    static bool led_on = false;

    // Check if the interrupt is within the debounce period
    if (time_before(jiffies, last_jiffies + msecs_to_jiffies(DEBOUNCE_DELAY_MS))) {
        return IRQ_HANDLED;  // Ignore spurious interrupt
    }

    // Update last handled jiffies
    last_jiffies = jiffies;

    // Toggle LED state
    led_on = !led_on;
    gpio_set_value(GPIO_LED, led_on ? 1 : 0);

    printk(KERN_INFO "Button pressed, LED %s\n", led_on ? "ON" : "OFF");
    pr_info("interrupt triggered and handled\n");
    return IRQ_HANDLED;
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("GPIO Interrupt Driver with Debouncing");
MODULE_VERSION("1.1");

