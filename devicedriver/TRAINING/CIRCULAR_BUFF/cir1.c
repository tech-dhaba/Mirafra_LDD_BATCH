#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_device"
#define MAX_ENTRIES 5
#define BUFFER_SIZE 1056 //definig the buffer size for the device
struct logs {
    int key;
    char value[50];
};

// Circular buffer
static struct logs kv[MAX_ENTRIES];
static int head = 0;  // Points to the next write position
static int tail = 0;  // Points to the next read position
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
            len +=snprintf(temp_buffer + len, BUFFER_SIZE - len, "%d %s\n",kv[j].key, kv[j].value);
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
    struct logs input;

    if (size != sizeof(struct logs)) {
        printk(KERN_ERR "simple_device: Invalid write size\n");
        return -EINVAL;
    }

    if (copy_from_user(&input, user_buffer, size)) {
        return -EFAULT;
    }

    // Write data into the buffer
    kv[head] = input;
    printk(KERN_INFO "simple_device: Received key=%d, value=%s\n", input.key, input.value);

    head = (head + 1) % MAX_ENTRIES; // Update head index in circular fashion

    if (count == MAX_ENTRIES) {
        // Overwrite the oldest entry if buffer is full
        tail = (tail + 1) % MAX_ENTRIES;
    } else {
        count++;
    }

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

