#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_driver"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE]; // Buffer to hold the doubled value
static int device_open = 0; // Flag to track device open status

// Function to handle opening the device
static int device_open_func(struct inode *inode, struct file *file) {
    if (device_open) {
        return -EBUSY; // Return "Device busy" error if the device is already open
    }
    device_open++; // Increment the device open count
    printk(KERN_INFO "Device opened\n");
    return 0; // Successful open
}

// Function to handle closing the device
static int device_release_func(struct inode *inode, struct file *file) {
    device_open--; // Decrement the device open count
    printk(KERN_INFO "Device released\n");
    return 0; // Successful release
}

// Function to handle reading from the device
static ssize_t device_read_func(struct file *filp, char __user *buffer, size_t len, loff_t *offset) {
    size_t data_len = sizeof(int); // We are reading one integer at a time

    // If we have already sent the data, return 0 to indicate end of file
    if (*offset >= data_len) {
        return 0;
    }

    // If the requested size is larger than the available data, adjust it
    if (len > data_len - *offset) {
        len = data_len - *offset;
    }

    // Copy data from kernel space to user space
    if (copy_to_user(buffer, device_buffer, len)) {
        return -EFAULT; // Return error if copying fails
    }

    *offset += len; // Update the offset for future reads
    printk(KERN_INFO "Device read: Sent %zu bytes\n", len);
    return len; // Return the number of bytes read
}

// Function to handle writing to the device
static ssize_t device_write_func(struct file *filp, const char __user *buffer, size_t len, loff_t *offset) {
    int number;

    // Check if the size of data being written is correct (must be an integer)
    if (len != sizeof(int)) {
        printk(KERN_ALERT "Invalid write size: Expected %zu bytes, got %zu bytes.\n", sizeof(int), len);
        return -EINVAL; // Return "Invalid argument" error
    }

    // Copy the integer from user space to kernel space
    if (copy_from_user(&number, buffer, sizeof(int))) {
        printk(KERN_ALERT "Failed to copy data from user space.\n");
        return -EFAULT; // Return error if copy fails
    }

    // Double the number as per your requirement
    number *= 2;

    // Store the doubled number in the kernel buffer (device_buffer)
    *((int *)device_buffer) = number;

    printk(KERN_INFO "Device write: Received %d, doubled to %d\n", number / 2, number);

    return sizeof(int); // Return the number of bytes written (size of int)
}

// File operations structure
static struct file_operations fops = {
    .open = device_open_func,
    .release = device_release_func,
    .read = device_read_func,
    .write = device_write_func,  // Linking the write function
};

// Module initialization function
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops); // Register the device
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major_number; // Return error if registration fails
    }

    printk(KERN_INFO "Device registered: %s with major number %d\n", DEVICE_NAME, major_number);
    return 0; // Successful initialization
}

// Module exit function
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME); // Unregister the device
    printk(KERN_INFO "Device unregistered\n");
}

module_init(simple_driver_init); // Register the initialization function
module_exit(simple_driver_exit); // Register the cleanup function

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver that doubles an integer");

