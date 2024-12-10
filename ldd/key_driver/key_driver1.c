#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "key_value_driver"
#define BUFFER_SIZE 256

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int k[10], v[10];
static struct mutex buffer_lock;

// Prototypes
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
=======
#include <linux/uaccess.h> // For copy_to_user and copy_from_user
#include <linux/init.h>    // For __init and __exit macros

#define DEVICE_NAME "simple_driver"
#define BUFFER_SIZE 2048

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9

// File operations structure
static struct file_operations fops = {
    .open = device_open,
<<<<<<< HEAD
    .release = device_release,
    .write = device_write,
    .read = device_read,
};

// Open device
static int device_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "key_value_driver: Device opened\n");
    return 0;
}

// Close device
static int device_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "key_value_driver: Device closed\n");
    return 0;
}

// Write key-value pairs
static ssize_t device_write(struct file *filep, const char __user *user_buffer, size_t size, loff_t *offset) {
    size_t write_offset=0,  total_pairs = 0;

    if (size > BUFFER_SIZE - 1)
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        printk(KERN_ERR "Failed to copy data from user\n");
        return -EFAULT;
    }

    device_buffer[size] = '\0';
    printk(KERN_INFO "Received %zu bytes: %s\n", size, device_buffer);

    while (write_offset < size && total_pairs < 10) { // Limit to 10 pairs
        int key, value;
        int ret = sscanf(device_buffer + write_offset, "%d:%d", &key, &value);
        if (ret != 2) {
            printk(KERN_WARNING "Invalid input at offset %zu\n", write_offset);
            return -EFAULT;
        }
        k[total_pairs] = key;
        v[total_pairs] = value;

        total_pairs++;
        write_offset += strcspn(device_buffer + write_offset, " ") + 1; // Advance to next pair
    }

    for (size_t i = 0; i < total_pairs; i++) {
        printk(KERN_INFO "Key[%zu]: %d, Value[%zu]: %d\n", i, k[i], i, v[i]);
    }

=======
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
    size_t len = strlen(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);

    char buf[10];
    int j = 0;
    for (int i = 0; i < strlen(device_buffer); i++) {
        if (device_buffer[i] != ':' && device_buffer[i] != ',') {
            buf[j++] = device_buffer[i];
        }
    }
    buf[j] = '\0';

    printk(KERN_INFO "Processed Data: %s\n", buf);
>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9
    return size;
}


<<<<<<< HEAD
// Read key-value pairs
static ssize_t device_read(struct file *filep, char __user *user_buffer, size_t size, loff_t *offset) {
    char temp_buffer[BUFFER_SIZE];
    size_t output_len = 0;

    mutex_lock(&buffer_lock);

    for (int i = 0; i < 10; i++) {
        output_len += snprintf(temp_buffer + output_len, BUFFER_SIZE - output_len, "%d:%d\n", k[i], v[i]);
        if (output_len >= BUFFER_SIZE) {
            break;
        }
    }

    mutex_unlock(&buffer_lock);

    if (copy_to_user(user_buffer, temp_buffer, output_len)) {
        return -EFAULT;
    }

    return output_len;
}

// Initialize the module
static int __init key_value_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "key_value_driver: Failed to register device\n");
        return major_number;
    }

    mutex_init(&buffer_lock);

    printk(KERN_INFO "key_value_driver: Registered with major number %d\n", major_number);
    return 0;
}

// Exit the module
static void __exit key_value_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    mutex_destroy(&buffer_lock);
    printk(KERN_INFO "key_value_driver: Unregistered device\n");
}

module_init(key_value_driver_init);
module_exit(key_value_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Key-Value Pair Driver");
=======
// Module initialization
static int __init  simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit  simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver");

>>>>>>> f907368958ad71acb2d78479ed73e4bdf26ccfb9

