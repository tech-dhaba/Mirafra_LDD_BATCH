#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "rw_driver"
#define BUFFER_SIZE 100

MODULE_LICENSE("GPL");

static char buffer[BUFFER_SIZE];
static int device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset) {
    if (copy_to_user(user_buffer, buffer, strlen(buffer) + 1)) {
        return -EFAULT;
    }
    return strlen(buffer) + 1;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset) {
    if (len > BUFFER_SIZE - 1)
        return -EINVAL;

    if (copy_from_user(buffer, user_buffer, len))
        return -EFAULT;

    buffer[len] = '\0';
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int major;

static int __init rw_driver_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("Failed to register character device\n");
        return major;
    }
    pr_info("RW driver loaded with major number %d\n", major);
    return 0;
}

static void __exit rw_driver_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("RW driver unloaded\n");
}

module_init(rw_driver_init);
module_exit(rw_driver_exit);

