#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ioctl_driver"
#define IOCTL_MAGIC 'x'
#define IOCTL_SET_STRING _IOW(IOCTL_MAGIC, 1, char *)
#define IOCTL_GET_STRING _IOR(IOCTL_MAGIC, 2, char *)

#define BUFFER_SIZE 100

MODULE_LICENSE("GPL");

static char buffer[BUFFER_SIZE];

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case IOCTL_SET_STRING:
            if (copy_from_user(buffer, (char __user *)arg, BUFFER_SIZE))
                return -EFAULT;
            return 0;

        case IOCTL_GET_STRING:
            if (copy_to_user((char __user *)arg, buffer, BUFFER_SIZE))
                return -EFAULT;
            return 0;

        default:
            return -EINVAL;
    }
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = device_ioctl,
};

static int major;

static int __init ioctl_driver_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("Failed to register character device\n");
        return major;
    }
    pr_info("IOCTL driver loaded with major number %d\n", major);
    return 0;
}

static void __exit ioctl_driver_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("IOCTL driver unloaded\n");
}

module_init(ioctl_driver_init);
module_exit(ioctl_driver_exit);

