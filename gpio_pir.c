#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define GPIO_PIR 529  // Pin number on Raspberry Pi GPIO
#define GPIO_LED 530		       

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sushma");
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
    int value = gpio_get_value(GPIO_PIR);  // Read GPIO pin 
		if(value==1)
		{
			
        gpio_set_value(GPIO_LED, value);  // Set GPIO pin value
        pr_info("GPIO Device write: %d\n", value);
		}
		else if(value==0){

        gpio_set_value(GPIO_LED, value);  // Set GPIO pin value
        pr_info("GPIO Device write: %d\n", value);
		}
    char buffer[2];
    snprintf(buffer, 2, "%d", value);

    if (copy_to_user(buf, buffer, 2)) {
        pr_err("Failed to send data to user\n");
        return -EFAULT;
    }

    pr_info("GPIO Device read: %d\n", value);
    return 2;  // Number of bytes sent
}


static struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .release = gpio_close,
    .read = gpio_read,
};

static int __init gpio_driver_init(void) {
    int ret,ret1;

    // Request GPIO pin
    ret = gpio_request(GPIO_PIR, "GPIO_DRIVER");
    ret1 = gpio_request(GPIO_LED, "GPIO_DRIVER");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIR);
        return -EBUSY;
    }
    if (ret1) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_LED);
        return -EBUSY;
    }

    // Set GPIO direction to output
    ret = gpio_direction_input(GPIO_PIR);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIR);
        gpio_free(GPIO_PIR);
        return ret;
    }

    ret1 = gpio_direction_output(GPIO_LED, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_LED);
        gpio_free(GPIO_LED);
        return ret;
    }

    // Register device
    ret = register_chrdev(0, "gpio_driver", &gpio_fops);
    pr_info("major num=%d",ret);
    if (ret < 0) {
        pr_err("Failed to register character device\n");
        gpio_free(GPIO_PIR);
        gpio_free(GPIO_LED);
        return ret;
    }

    pr_info("GPIO Driver loaded\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
    unregister_chrdev(0, "gpio_driver");
    gpio_free(GPIO_PIR);
    gpio_free(GPIO_LED);
    pr_info("GPIO Driver unloaded\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);
