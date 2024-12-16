#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1056

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
static int write_index = 0; // Keep track of the current position in the circular buffer

// Define a circular buffer to hold messages
struct logs {
    char msg[100];
};
static struct logs svar[5]; // Buffer size is 5

// File operation prototypes
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

// Open function
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Close function
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Read function: Reads all stored messages from the circular buffer
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    int len = 0,i;
    char temp_buffer[BUFFER_SIZE] = {0};  // Temporary buffer to store logs

    // Concatenate all messages in the buffer
    for (i = 0; i < 5; i++) {
        if (strlen(svar[i].msg) > 0) {
            len += snprintf(temp_buffer + len, sizeof(temp_buffer) - len, "%s\n", svar[i].msg);
        }
    }

    // If there is no data to read, return 0 (EOF)
    if (len == 0) {
        return 0;
    }

    // Adjust size to avoid reading beyond the buffer
    if (*offset >= len)
        return 0;

    if (size > len - *offset)
        size = len - *offset;

    // Copy data from temp_buffer to the user buffer
    if (copy_to_user(user_buffer, temp_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Write function: Writes a new message into the circular buffer
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1)  // Limit the size to the buffer capacity
        size = BUFFER_SIZE - 1;
	
    char user_message[BUFFER_SIZE];
    char timestamped_message[BUFFER_SIZE];
	char timestamp[64];
    struct timespec64 ts;
    struct tm tm;

    ktime_get_real_ts64(&ts);                // Get current real-time
  	time64_to_tm(ts.tv_sec, 0, &tm);         // Convert to broken-down time
    snprintf(timestamp,sizeof(timestamp), "date:%02d:%02d:%04ld time:%02d:%02d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour+6, tm.tm_min);

    // Copy data from user space to the kernel buffer
    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    // Null-terminate the string
    device_buffer[size] = '\0';

    // Store the message in the circular buffer at the current write_index
    snprintf(svar[write_index].msg, sizeof(svar[write_index].msg), "%s %s", timestamp,device_buffer);
    printk(KERN_INFO "simple_device: Received msg='%s'\n", svar[write_index].msg);

    // Increment write_index and wrap around using modulo to simulate circular buffer behavior
    write_index = (write_index + 1) % 5;

    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
    return size;
}

// Module initialization function
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module exit function
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver");

