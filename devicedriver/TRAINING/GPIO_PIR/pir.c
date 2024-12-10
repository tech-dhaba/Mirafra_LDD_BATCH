#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define GPIO_PIN 529  // Pin number on Raspberry Pi GPIO
#define GPIO_PIN1 534


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashritha");
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
    int value = gpio_get_value(GPIO_PIN);  // Read GPIO pin value
    char buffer[2];
    snprintf(buffer, 2, "%d", value);//17 is written to buffer

    if (copy_to_user(buf, buffer, 2)) {//copied to user
        pr_err("Failed to send data to user\n");
        return -EFAULT;
    }

    pr_info("GPIO Device read: %d\n", value);
    return 2;  // Number of bytes sent
}

static ssize_t gpio_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    char buffer[2];
	int value;
    if (copy_from_user(buffer, buf, len)) {//taking daata frim user
        pr_err("Failed to receive data from user\n");
        return -EFAULT;
    }

    buffer[len] = '\0';
    value = simple_strtol(buffer, NULL, 10);//convert to string

    if (value == 0 || value == 1) {
        gpio_set_value(GPIO_PIN, value);  // Set GPIO pin value
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

static int __init gpio_driver_init(void) {//called when insmod
    int ret,ret1;

    // Request GPIO pin
    ret = gpio_request(GPIO_PIN, "GPIO_DRIVER");//request to kernel
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN);
        return ret;
    }
	ret1 = gpio_request(GPIO_PIN1, "GPIO_DRIVER");//request to kernel
    if (ret1) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN1);
        return ret1;
    }


    // Set GPIO direction to output
    ret = gpio_direction_output(GPIO_PIN, 0);//direction
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN);
        gpio_free(GPIO_PIN);
        return ret;
    }
	ret1 = gpio_direction_input(GPIO_PIN1);//direction
    if (ret1) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN1);
        gpio_free(GPIO_PIN1);
        return ret1;
    }


    // Register device
    ret = register_chrdev(0, "gpio_driver", &gpio_fops);//forcefully givig major number 240   if u give 0 kernel will give
    if (ret < 0) {
        pr_err("Failed to register character device\n");
        gpio_free(GPIO_PIN);
        return ret;
    }

    pr_info("GPIO Driver loaded\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
    unregister_chrdev(0, "gpio_driver");//unregister
    gpio_free(GPIO_PIN);
    pr_info("GPIO Driver unloaded\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

