#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/slab.h>

#define UART_PORT "/dev/serial1" /* UART device name */

static struct file *uart_file = NULL;  // Global file pointer for UART device

// File operation prototypes
static int uart_open(struct inode *inode, struct file *file);
static int uart_release(struct inode *inode, struct file *file);
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// Define UART file operations
static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .open = uart_open,
    .release = uart_release,
    .read = uart_read,
    .write = uart_write,
};

// Open UART device
static int uart_open(struct inode *inode, struct file *file) {
    if (uart_file) {
        pr_err("UART device already opened\n");
        return -EBUSY;
    }

    uart_file = filp_open(UART_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_file)) {
        pr_err("Failed to open UART device %s\n", UART_PORT);
        uart_file = NULL;
        return PTR_ERR(uart_file);
    }

    pr_info("UART device opened successfully\n");
    return 0;
}

// Close UART device
static int uart_release(struct inode *inode, struct file *file) {
    if (uart_file) {
        filp_close(uart_file, NULL);
        uart_file = NULL;
        pr_info("UART device closed\n");
    }
    return 0;
}

// Read from UART device
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    char *kbuf;
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;
    }

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf) {
        pr_err("Failed to allocate kernel buffer\n");
        return -ENOMEM;
    }

    ret = kernel_read(uart_file, kbuf, len, offset);
    if (ret < 0) {
        pr_err("Failed to read from UART device\n");
        kfree(kbuf);
        return ret;
    }

    if (copy_to_user(buf, kbuf, ret)) {
        pr_err("Failed to copy data to user space\n");
        kfree(kbuf);
        return -EFAULT;
    }

    pr_info("Read %zd bytes from UART device\n", ret);
    kfree(kbuf);
    return ret;
}

// Write to UART device
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    char *kbuf;
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;
    }

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf) {
        pr_err("Failed to allocate kernel buffer\n");
        return -ENOMEM;
    }

    if (copy_from_user(kbuf, buf, len)) {
        pr_err("Failed to copy data from user space\n");
        kfree(kbuf);
        return -EFAULT;
    }

    ret = kernel_write(uart_file, kbuf, len, offset);
    if (ret < 0) {
        pr_err("Failed to write to UART device\n");
    } else {
        pr_info("Written %zd bytes to UART device\n", ret);
    }

    kfree(kbuf);
    return ret;
}

// Module init function
static int __init uart_init(void) {
    int ret;

    pr_info("Loading UART Module\n");

    ret = register_chrdev(0, "serial1", &uart_fops);
    if (ret < 0) {
        pr_err("Failed to register UART driver\n");
        return ret;
    }

    pr_info("UART driver registered successfully with major number %d\n", ret);
    return 0;
}

// Module exit function
static void __exit uart_exit(void) {
    pr_info("Unloading UART Module\n");
    unregister_chrdev(0, "uart_driver");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("A Robust UART Driver Module");
