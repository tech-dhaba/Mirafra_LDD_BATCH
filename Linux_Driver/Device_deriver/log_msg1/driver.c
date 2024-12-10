#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/ktime.h>

#define DEVICE_NAME "logger_device"
#define BUFFER_SIZE 256
#define MAX_LOGS 5

static int major_number;
static int open_count = 0;
static int write_index = 0;
static int read_index = 0;

struct log_entry {
    char log_message[BUFFER_SIZE];
};

static struct log_entry log_buffer[MAX_LOGS];

// Function prototypes
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

// Device open function
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "logger_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Device close function
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "logger_device: Device closed\n");
    return 0;
}

// Device write function
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    char user_message[BUFFER_SIZE];
    char timestamped_message[BUFFER_SIZE];
    struct timespec64 ts;
    char timestamp[64];

    // Get the current timestamp
    ktime_get_real_ts64(&ts);
    snprintf(timestamp, sizeof(timestamp), "%04lld-%02lld-%02lld %02lld:%02lld:%02lld",
             (1900 + ts.tv_sec / 31556926), // Year approximation
             ((ts.tv_sec / 2629743) % 12 + 1), // Month
             ((ts.tv_sec / 86400) % 30 + 1), // Day
             (ts.tv_sec / 3600) % 24,       // Hour
             (ts.tv_sec / 60) % 60,         // Minute
             ts.tv_sec % 60);              // Second

    // Copy message from user space
    if (copy_from_user(user_message, user_buffer, size)) {
        return -EFAULT;
    }
    user_message[size] = '\0'; // Null-terminate the user message

    // Combine timestamp with the user message
    snprintf(timestamped_message, sizeof(timestamped_message), "[%s] %s", timestamp, user_message);

    // Store the message in the circular buffer
    snprintf(log_buffer[write_index].log_message, BUFFER_SIZE, "%s", timestamped_message);
    write_index = (write_index + 1) % MAX_LOGS;

    printk(KERN_INFO "logger_device: Log stored: %s\n", timestamped_message);
    return size;
}

// Device read function
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t log_len;
    if (read_index == write_index) {
        printk(KERN_INFO "logger_device: No logs to read\n");
        return 0; // No logs available
    }

    // Get the next log from the circular buffer
    log_len = strlen(log_buffer[read_index].log_message);
    if (copy_to_user(user_buffer, log_buffer[read_index].log_message, log_len)) {
        return -EFAULT;
    }
    read_index = (read_index + 1) % MAX_LOGS;

    printk(KERN_INFO "logger_device: Log read\n");
    return log_len;
}

// Module initialization
static int __init logger_device_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "logger_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "logger_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit logger_device_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "logger_device: Unregistered device\n");
}

module_init(logger_device_init);
module_exit(logger_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Logger Device Driver with Timestamped Messages");

