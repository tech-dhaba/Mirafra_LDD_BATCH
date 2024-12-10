#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 128

// Arithmetic functions
static int add(int a, int b) {
    return a + b;
}

static int sub(int a, int b) {
    return a - b;
}

static int mul(int a, int b) {
    return a * b;
}

// Structure with function pointers
struct student {
    int (*fp_add)(int, int);
    int (*fp_sub)(int, int);
    int (*fp_mul)(int, int);
};

// Global structure instance
static struct student s;

// Device variables
static int major_number;
static char device_buffer[BUFFER_SIZE];
static int result = 0;

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

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device opened\n");
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    char result_buffer[BUFFER_SIZE];
    int len = snprintf(result_buffer, BUFFER_SIZE, "Result: %d\n", result);

    if (*offset >= len)
        return 0;

    if (size > len - *offset)
        size = len - *offset;

    if (copy_to_user(user_buffer, result_buffer + *offset, size))
        return -EFAULT;

    *offset += size;
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    int a, b;
    char operation;

    if (size > BUFFER_SIZE - 1)
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size))
        return -EFAULT;

    device_buffer[size] = '\0';

    // Parse user input (e.g., "a b +")
    if (sscanf(device_buffer, "%d %d %c", &a, &b, &operation) != 3) {
        printk(KERN_WARNING "simple_device: Invalid input format\n");
        return -EINVAL;
    }

    // Call the appropriate function based on the operation
    switch (operation) {
        case '+':
            result = s.fp_add(a, b);
            break;
        case '-':
            result = s.fp_sub(a, b);
            break;
        case '*':
            result = s.fp_mul(a, b);
            break;
        default:
            printk(KERN_WARNING "simple_device: Unknown operation '%c'\n", operation);
            return -EINVAL;
    }

    printk(KERN_INFO "simple_device: Operation %c on %d and %d resulted in %d\n", operation, a, b, result);
    return size;
}

// Module initialization
static int __init simple_driver_init(void) {
    // Initialize function pointers in the structure
    s.fp_add = add;
    s.fp_sub = sub;
    s.fp_mul = mul;

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
MODULE_DESCRIPTION("Linux Device Driver with Struct Function Pointers");

