#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  // For copy_to_user and copy_from_user
#include <linux/ioctl.h>

#define DEVICE_NAME "my_device"
#define IOCTL_MAGIC  'M'   // Magic number to identify the ioctl commands

// Define ioctl commands
#define IOCTL_SET_VALUE _IOW(IOCTL_MAGIC, 0, int)  // Write value to kernel
#define IOCTL_GET_VALUE _IOR(IOCTL_MAGIC, 1, int)  // Read value from kernel

static int major_number;
static int kernel_value = 0;  // Variable to store value passed by user

// Forward declaration of ioctl function
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl,
};

// Implementation of ioctl function
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    switch (cmd) {
        case IOCTL_SET_VALUE:
            // Write integer from user space to kernel space
            if (copy_from_user(&kernel_value, (int __user *)arg, sizeof(int))) {
                ret = -EFAULT;
            }
            printk(KERN_INFO "IOCTL_SET_VALUE: Set value to %d\n", kernel_value);
            break;

        case IOCTL_GET_VALUE:
            // Send the value from kernel space to user space
            if (copy_to_user((int __user *)arg, &kernel_value, sizeof(int))) {
                ret = -EFAULT;
            }
            printk(KERN_INFO "IOCTL_GET_VALUE: Sent value %d\n", kernel_value);
            break;

        default:
            ret = -EINVAL;  // Invalid command
            printk(KERN_WARNING "Unknown ioctl command\n");
            break;
    }

    return ret;
}

// Module initialization function
static int __init my_driver_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "my_driver: Device registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup function
static void __exit my_driver_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "my_driver: Device unregistered\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Linux Driver with ioctl Commands");

