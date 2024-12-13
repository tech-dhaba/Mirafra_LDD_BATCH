#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

struct kv{
int key;
int value;
};

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
static struct kv kv_a[10];
static int kv_count=0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = device_open,                        //function pointer to open the device 
    .release = device_close,			//function pointer to close device
    .read = device_read,			//function pointer to read from user space
    .write = device_write,			//function pointer to write to user space
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
    int key,i;
    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;
	
    if (copy_from_user(device_buffer, user_buffer, size)) {
    	return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
    
    key=simple_strtol(device_buffer,NULL,10);

    for(i=0;i<kv_count;i++){
	if(kv_a[i].key==key){
		snprintf(device_buffer,BUFFER_SIZE,"%d,%d",kv_a[i].key,kv_a[i].value);
    		if (copy_to_user(user_buffer, device_buffer, 200)) {
        		return -EFAULT;
		}
	}
    }


    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    
    if(kv_count>=10)
    {
	printk(KERN_INFO "Buffer full\n");
	return -EFAULT;
    }

    int i;
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
    size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
    
    for(i=0;device_buffer[i]!=',';i++);
    kv_a[kv_count].key=simple_strtol(device_buffer,NULL,10);
    kv_a[kv_count].value=simple_strtol(device_buffer+i+1,NULL,10);
    kv_count++;

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
