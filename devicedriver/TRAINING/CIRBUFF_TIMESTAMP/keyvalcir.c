
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_device"
#define MAX_ENTRIES 5
#define BUFFER_SIZE 256 //definig the buffer size for the device
struct logs {
   // int key;
    char value[BUFFER_SIZE];
};
static int write_index = 0;
#define MAX_LOGS 5
// Circular bufferi
static char device_buffer[BUFFER_SIZE];
static struct logs kv[MAX_ENTRIES];
static int count = 0; // Number of valid entries in the buffer
static int open_count = 0; // Device open counter
static int major_number;

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
    open_count++;
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    int len = 0,j=0;
    char temp_buffer[BUFFER_SIZE] = {0}; // Temporary buffer to store logs
   for (j=0;j<MAX_ENTRIES;j++)
    {
        if (strlen(kv[j].value) > 0) {
            len +=snprintf(temp_buffer + len, BUFFER_SIZE - len, "%s\n", kv[j].value);
        }
    }

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(user_buffer, temp_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1)  // Limit the size to the buffer capacity
        size = BUFFER_SIZE - 1;
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
    snprintf(kv[write_index].value, sizeof(kv[write_index].value), "%s %s", timestamp,device_buffer);
    printk(KERN_INFO "simple_device: Received msg='%s'\n", kv[write_index].value);

    // Increment write_index and wrap around using modulo to simulate circular buffer behavior
    write_index = (write_index + 1) % 5;

    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
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
MODULE_DESCRIPTION("A Simple Linux Device Driver");

