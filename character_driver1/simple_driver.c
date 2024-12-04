#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user
#include <linux/string.h>
#include <linux/printk.h>
#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);
static loff_t device_lseek(struct file *file, loff_t pos, int whence);

// File operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
	.llseek = device_lseek
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
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size + *offset  > BUFFER_SIZE ) // Limit size to buffer capacity
        size = BUFFER_SIZE - *offset;

    if (copy_from_user(device_buffer + *offset, user_buffer, size)) {
        return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
	pr_info("Before updating offset....file -> f_pos =%lld , *offset = %lld", file -> f_pos, *offset);
	*offset += size;
	pr_info("After updating offset....file -> f_pos =%lld , *offset = %lld", file -> f_pos, *offset);
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", sizeof(device_buffer));
	return size;
}
static loff_t device_lseek(struct file *file, loff_t offset, int whence)
{
	loff_t temp;
	pr_info("lseek requested\n");
	switch(whence)
	{
		case SEEK_SET:
			if((offset > BUFFER_SIZE) || (offset < 0))
				return -EINVAL;
			file->f_pos = offset;
			break;
		case SEEK_CUR:
			temp=file->f_pos + offset;
			if((temp>BUFFER_SIZE) || (temp <0))
				return -EINVAL;
			file->f_pos = temp;
			break;
		case SEEK_END:
			temp=BUFFER_SIZE + offset;
			if((temp>BUFFER_SIZE) || (temp<0))
				return -EINVAL;
			file->f_pos = temp;
			break;
		default :
			return -EINVAL;
	}
	return file->f_pos;
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
