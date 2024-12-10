#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

<<<<<<< HEAD
#define GPIO_PIN1 17  // Pin number on Raspberry Pi GPIO
#define GPIO_PIN2 18  // Pin number on Raspberry Pi GPIO
=======
#define GPIO_PIN 17  // Pin number on Raspberry Pi GPIO
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple GPIO Driver for Raspberry Pi");
MODULE_VERSION("1.0");

static int gpio_open(struct inode *inode, struct file *file) {
    pr_info("GPIO Device opened\n");
    return 0;
}

static int gpio_close(struct inode *inode, struct file *file) {
    pr_info("GPIO Device closed\n");
    return 0;
}

static ssize_t gpio_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
<<<<<<< HEAD
    int value = gpio_get_value(GPIO_PIN1);  // Read GPIO pin value
    char buffer[2];
    snprintf(buffer, sizeof(buffer), "%d", value);

    if (copy_to_user(buf, buffer, sizeof(buffer))) {
=======
    int value = gpio_get_value(GPIO_PIN);  // Read GPIO pin value
    char buffer[2];
    snprintf(buffer, 2, "%d", value);

    if (copy_to_user(buf, buffer, 2)) {
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
        pr_err("Failed to send data to user\n");
        return -EFAULT;
    }

    pr_info("GPIO Device read: %d\n", value);
<<<<<<< HEAD
    return sizeof(buffer);  // Number of bytes sent
=======
    return 2;  // Number of bytes sent
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
}

static ssize_t gpio_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    char buffer[2];
<<<<<<< HEAD
    int value;
=======
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9

    if (copy_from_user(buffer, buf, len)) {
        pr_err("Failed to receive data from user\n");
        return -EFAULT;
    }

<<<<<<< HEAD
    // Null-terminate the buffer to safely convert to an integer
    buffer[len] = '\0';

    // Convert string to integer (0 or 1)
    value = simple_strtol(buffer, NULL, 10);

    if (value == 0 || value == 1) {
        gpio_set_value(GPIO_PIN2, value);  // Set GPIO pin value
=======
    buffer[len] = '\0';
    int value = simple_strtol(buffer, NULL, 10);

    if (value == 0 || value == 1) {
        gpio_set_value(GPIO_PIN, value);  // Set GPIO pin value
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
        pr_info("GPIO Device write: %d\n", value);
    } else {
        pr_err("Invalid value: GPIO accepts 0 or 1\n");
        return -EINVAL;
    }

    return len;
}

static struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .release = gpio_close,
    .read = gpio_read,
    .write = gpio_write,
};

static int __init gpio_driver_init(void) {
    int ret;

<<<<<<< HEAD
    // Request GPIO pins
    ret = gpio_request(GPIO_PIN1, "GPIO_DRIVER1");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN1);
        return ret;
    }

    ret = gpio_request(GPIO_PIN2, "GPIO_DRIVER2");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN2);
        gpio_free(GPIO_PIN1);  // Free previously requested pin
        return ret;
    }

    // Set GPIO direction to output for both pins
    ret = gpio_direction_output(GPIO_PIN1, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN1);
        gpio_free(GPIO_PIN1);
        gpio_free(GPIO_PIN2);
        return ret;
    }

    ret = gpio_direction_output(GPIO_PIN2, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN2);
        gpio_free(GPIO_PIN1);
        gpio_free(GPIO_PIN2);
        return ret;
    }

    // Register the device with the kernel
    ret = register_chrdev(240, "gpio_driver", &gpio_fops);
    if (ret < 0) {
        pr_err("Failed to register character device\n");
        gpio_free(GPIO_PIN1);
        gpio_free(GPIO_PIN2);
=======
    // Request GPIO pin
    ret = gpio_request(GPIO_PIN, "GPIO_DRIVER");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN);
        return ret;
    }

    // Set GPIO direction to output
    ret = gpio_direction_output(GPIO_PIN, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN);
        gpio_free(GPIO_PIN);
        return ret;
    }

    // Register device
    ret = register_chrdev(240, "gpio_driver", &gpio_fops);
    if (ret < 0) {
        pr_err("Failed to register character device\n");
        gpio_free(GPIO_PIN);
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
        return ret;
    }

    pr_info("GPIO Driver loaded\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
    unregister_chrdev(240, "gpio_driver");
<<<<<<< HEAD
    gpio_free(GPIO_PIN1);
    gpio_free(GPIO_PIN2);
=======
    gpio_free(GPIO_PIN);
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
    pr_info("GPIO Driver unloaded\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);
<<<<<<< HEAD

=======
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
