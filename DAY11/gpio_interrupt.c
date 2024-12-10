#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/errno.h>

#define GPIO_PIN 17  // Replace with the GPIO pin number you want to use

static unsigned int irq_number;
static bool led_state = false;

// Interrupt handler function
static irqreturn_t gpio_irq_handler(int irq, void *dev_id) {
    pr_info("GPIO Interrupt triggered!\n");

    // Toggle LED state or perform your desired operation
    led_state = !led_state;
    gpio_set_value(GPIO_PIN, led_state);

    return IRQ_HANDLED;
}

static int __init gpio_interrupt_init(void) {
    int ret;

    pr_info("GPIO Interrupt Module Loading\n");

    // Request GPIO pin
    if (!gpio_is_valid(GPIO_PIN)) {
        pr_err("Invalid GPIO pin: %d\n", GPIO_PIN);
        return -ENODEV;
    }

    ret = gpio_request(GPIO_PIN, "sysfs");
    if (ret) {
        pr_err("Failed to request GPIO: %d\n", GPIO_PIN);
        return ret;
    }

    gpio_direction_output(GPIO_PIN, led_state);  // Set GPIO as output
    gpio_export(GPIO_PIN, false);

    // Request IRQ line
    irq_number = gpio_to_irq(GPIO_PIN);
    if (irq_number < 0) {
        pr_err("Failed to get IRQ number for GPIO: %d\n", GPIO_PIN);
        gpio_free(GPIO_PIN);
        return irq_number;
    }

    ret = request_irq(
        irq_number,                // IRQ number
        gpio_irq_handler,          // Interrupt handler function
        IRQF_TRIGGER_RISING,       // Trigger on rising edge
        "gpio_interrupt",         // Interrupt name
        NULL                       // Device ID (optional)
    );

    if (ret) {
        pr_err("Failed to request IRQ: %d\n", irq_number);
        gpio_free(GPIO_PIN);
        return ret;
    }

    pr_info("GPIO Interrupt Module Loaded Successfully\n");
    return 0;
}

static void __exit gpio_interrupt_exit(void) {
    pr_info("GPIO Interrupt Module Unloading\n");

    free_irq(irq_number, NULL);    // Free the IRQ
    gpio_unexport(GPIO_PIN);      // Unexport the GPIO
    gpio_free(GPIO_PIN);          // Release the GPIO

    pr_info("GPIO Interrupt Module Unloaded Successfully\n");
}

module_init(gpio_interrupt_init);
module_exit(gpio_interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple GPIO interrupt driver for Raspberry Pi");
MODULE_VERSION("1.0");
