#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/serial.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define UART_PORT "/dev/uart_driver" /* UART device file path */
#define UART_PORT_BASE 0x07e201000 /* Base address for the UART */
#define UART_BAUD_RATE 9600        /* Baud rate */
#define UART_CLOCK_FREQ 48000000   /* UART clock frequency (example value) */

/* UART Register Offsets (example for Raspberry Pi UART) */
#define UART_DR  0x00 /* Data Register */
#define UART_IBRD 0x24 /* Integer Baud Rate Divisor */
#define UART_FBRD 0x28 /* Fractional Baud Rate Divisor */
#define UART_LCRH 0x2C /* Line Control Register */
#define UART_CR  0x30 /* Control Register */
#define UART_IMSC 0x38 /* Interrupt Mask Set/Clear Register */

// File operation function prototypes
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

// Global file pointer for UART device
static struct file *uart_file = NULL;
static int major_num;

// UART Open function
static int uart_open(struct inode *inode, struct file *file)
{
    uart_file = filp_open(UART_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_file)) {
        pr_err("Failed to open UART device %s\n", UART_PORT);
        return PTR_ERR(uart_file);
    }

    pr_info("UART device opened successfully\n");
    return 0;
}

// UART Release function
static int uart_release(struct inode *inode, struct file *file)
    if (uart_file && !IS_ERR(uart_file)) {
        filp_close(uart_file, NULL);
        pr_info("UART device closed\n");
    }
    return 0;
}

// UART Read function
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    char *kbuf;
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;
    }

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;

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

    kfree(kbuf);
    pr_info("Read %zd bytes from UART device\n", ret);
    return ret;
}

// UART Write function
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    char *kbuf;
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;
    }

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;

    if (copy_from_user(kbuf, buf, len)) {
        pr_err("Failed to copy data from user space\n");
        kfree(kbuf);
        return -EFAULT;
    }

    ret = kernel_write(uart_file, kbuf, len, NULL);
    kfree(kbuf);

    if (ret < 0) {
        pr_err("Failed to write to UART device\n");
        return ret;
    }

    pr_info("Written %zd bytes to UART device\n", ret);
    return ret;
}

// Module Init function
static int __init uart_init(void)
{
    pr_info("UART Module Loaded\n");

    // Register the character device
    major_num = register_chrdev(0, "uart_driver", &uart_fops);
    if (major_num < 0) {
        pr_err("Failed to register UART driver\n");
        return major_num;
    }

    pr_info("UART driver registered successfully with major number %d\n", major_num);
    return 0;
}

// Module Exit function
static void __exit uart_exit(void)
{
    pr_info("UART Module Unloaded\n");

    // Unregister the character device
    unregister_chrdev(major_num, "ttys0");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("A Simple UART Driver Module");

