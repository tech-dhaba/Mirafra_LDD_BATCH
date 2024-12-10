#include <linux/module.h>  // For module_init and module_exit
#include <linux/kernel.h>  // For printk
#include <linux/fs.h>      // For file operations
#include <linux/uaccess.h> // For copy_to_user() and copy_from_user()

#define DEVICE_NAME "simple_device"  // Define the device name that will appear on /dev
#define BUFFER_SIZE 1000  // Define the buffer size for the device
#define MAX_LOGS 5        // Maximum number of logs

// Define global variables
static int major_number;  // To store major device number
static int open_count = 0;  // To count the number of times the device is opened
static int index= 0;   // Index for circular buffer
static char device_buffer[BUFFER_SIZE];  // A buffer to store data for reading/writing

// Structure to hold key-value pairs
static struct abc {
    char key[50];
    int val;
}s[MAX_LOGS];  // Circular buffer to store logs

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    open_count++;  // Incrementing the open count each time the device is opened
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}
/*
// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = 0;
    int i;
    int bytes_sent = 0;

    // Concatenate all logs in the buffer into device_buffer
    for (i = 0; i < MAX_LOGS; i++) {
        len += snprintf(device_buffer + len, sizeof(device_buffer) - len, "key=%s val=%d\n",s[i].key,s[i].val);
    }

    // Check if all data has been read
    if (*offset >= len)
        return 0;  // No more data to send

    // Ensure we do not send more data than the user buffer can handle
    if (size > len - *offset)
        size = len - *offset;

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;  // Update the offset
    bytes_sent = size;
    printk(KERN_INFO "simple_device: Sent %d bytes to the user\n", bytes_sent);
    return bytes_sent;
}
*/
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strlen(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(device_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1)  // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    // Copy data from user space to kernel space
    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    // Parse the data (key and value)
    if (index < MAX_LOGS) {
      //  sscanf(device_buffer, "%s %d",s[index].key, &s[index].val);
        printk(KERN_INFO "simple_device: Received key=%s val=%d\n",s[index].key,s[index].val);
        index++;
    } else {
        // Buffer full, write "limit exceed" to device_buffer
        snprintf(device_buffer, sizeof(device_buffer), "limit exceed");
        printk(KERN_INFO "simple_device: Log limit exceeded\n");
    }

    // Wrap around the log index if needed (circular buffer)
    index %= MAX_LOGS;

    return size;
}

// Module initialization
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver with Circular Buffer");

