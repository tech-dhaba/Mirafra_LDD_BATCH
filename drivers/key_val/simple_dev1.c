//basic implementation of a Linux character device driver that can read and write data between user space and kernel space, while also providing simple logging and tracking functionality

#include <linux/module.h>//for module_init and module_exit
#include <linux/kernel.h>//for printk
#include <linux/fs.h> //for file operations
#include <linux/uaccess.h> // For copy_to_user() and copy_from_user()

#define DEVICE_NAME "simple_device" //defining the device name that will appear on /dev
#define BUFFER_SIZE 1000 //definig the buffer size for the device

//definig global variables
static int major_number; //to store major device number
static char device_buffer[BUFFER_SIZE]; //a buffer to store data for reading/writing
static int open_count = 0; //to count the no.of times the device is opened
static int count=0;
static char buff[]="limit exceed";
static int j=0;
// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

static struct abc
{
	char key[50];
	int val;
};
static struct abc s[10];

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
    char str[100];
	size_t len = strlen(str);
    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;
    snprintf(str,sizeof(str),"%s %d",s[j].key,s[j].val);
    j++;
    if (copy_to_user(user_buffer,str+strlen(str), size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

//    device_buffer[size] = '\0'; // Null-terminate the string
//    if(count<5)
  //  {
	if (copy_from_user(&s[count], user_buffer, size)) {
        	return -EFAULT;
    	}
 //   	sscanf(device_buffer,"%s %d",s[count].key,&s[count].val);
	printk(KERN_INFO "key is=%s\n val is=%d\n",s[count].key,s[count].val);
	sprintf(device_buffer,"%s %d",s[count].key,s[count].val);
	printk(KERN_INFO "bevice buffer=%s\n",device_buffer);
	count++;
	device_buffer[size] = '\0';
    //}
   // else
    	sprintf(device_buffer,"%s",buff);
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
