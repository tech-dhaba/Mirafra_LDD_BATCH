//basic implementation of a Linux character device driver that can read and write data between user space and kernel space, while also providing simple logging and tracking functionality

#include <linux/module.h>//for module_init and module_exit
#include <linux/kernel.h>//for printk
#include <linux/fs.h> //for file operations
#include <linux/uaccess.h> // For copy_to_user() and copy_from_user()

#define DEVICE_NAME "simple_device" //defining the device name that will appear on /dev
#define BUFFER_SIZE 1024 //definig the buffer size for the device

//definig global variables
static int major_number; //to store major device number
static char device_buffer[BUFFER_SIZE]; //a buffer to store data for reading/writing
static int open_count = 0; //to count the no.of times the device is opened

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
    open_count++;//incrementing the open count each time when device is opened
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
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    int a,b,res=0;
    char str[4];
	if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }
    device_buffer[size] = '\0'; // Null-terminate the string
    sscanf(device_buffer, "%d %d %s",&a,&b,str);
	str[3]='\0';
    if(strcmp(str,"add")==0)
	    res=a+b;
    if(strcmp(str,"sub")==0)
            res=a-b;
    if(strcmp(str,"mul")==0)
            res=a*b;
    sprintf(device_buffer,"res is %d",res);
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
