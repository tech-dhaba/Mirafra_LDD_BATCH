#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "key_value"
#define BUFFER_SIZE 1024

struct data {
	int key;
	int value;
};

static int major_number;
//static char device_buffer[BUFFER_SIZE];
static int device_buffer;
static struct data set[4];
static int open_count = 0;


// Function prototypes for device operations
void find_perfect_value(void);
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

//find the value for currsponding key
void find_perfect_value(void)
{
	for(int i = 0 ; i < 4 ;i++)
	{
		if(set[i].key == device_buffer)
		{
			device_buffer = set[i].value;
			return;
		}
	}
}

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "key_value: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "key_value: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = sizeof(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;
    /*To get the key value at the time of reading the data*/

    if(copy_from_user(&device_buffer,user_buffer,size))
    {
	    return -EFAULT;
    }
    //call fun to get value from provided key : 
    
    find_perfect_value();

    if(copy_to_user(user_buffer, &device_buffer, size)) {
        return -EFAULT;
    }
   // pr_info("before = %d\n",*offset);
    *offset += size;
   // pr_info("after = %d\n",*offset);
    printk(KERN_INFO "key_value: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(set, user_buffer, size)) {
        return -EFAULT;
    }

    //device_buffer[size] = '\0'; // Null-terminate the string
    printk(KERN_INFO "key_value: Received %zu bytes from the user\n", size);
    //pr_info("received data from user space = %d\n",device_buffer);
    pr_info("KEY : VALUE = %d : %d , %d : %d , %d : %d , %d : %d\n",set[0].key,set[0].value,set[1].key,set[1].value,set[2].key,set[2].value,set[3].key,set[3].value);
    //find_perfect_value();
    return size;
}

// Module initialization
static int __init key_value_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "key_value: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "key_value: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit key_value_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "key_value: Unregistered device\n");
}

module_init(key_value_init);
module_exit(key_value_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajini");
MODULE_DESCRIPTION("A Simple Linux Device Driver to store key-value");
