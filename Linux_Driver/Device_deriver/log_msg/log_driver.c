#include <linux/module.h>	//
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user
#include <linux/time.h>    // For current_kernel_time()

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024
#define max_size 5
static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0,i=0,j=0;
struct log_msg
{
	char log_message[256];
};
static struct log_msg s[5];
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

	snprintf(device_buffer,sizeof(device_buffer),"%s",s[j].log_message);
	printk(KERN_INFO "device_buffer:%s",device_buffer);
    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }
	j=((j+1)%sizeof(max_size));
	printk(KERN_INFO "j=%d",j);

    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    char timestramped_msg[256];
    struct timespec64 ts;
    char timestamp[64];

    // Get the current timestamp
    ktime_get_real_ts64(&ts);
   snprintf(date_time_str, sizeof(date_time_init), "%04lld-%02lld-%02lld %02lld:%02lld:%02lld",
             (1900 + ts.tv_sec / 31556926),  // Approximate year
             ((ts.tv_sec / 2629743) % 12 + 1), // Month
             ((ts.tv_sec / 86400) % 30 + 1),   // Day
             (ts.tv_sec / 3600) % 24,         // Hour
             (ts.tv_sec / 60) % 60,           // Minute
             ts.tv_sec % 60);                // Second
    if (copy_from_user(&s[i].log_message, user_buffer, size)) {
        return -EFAULT;
    }
	//printk(KERN_INFO "i=%d",i);
   	snprintf(timestramped_msg,sizeof(timestramped_msg),"%s %s",timestamp,s[i].log_message);
    snprintf(s[i].log_message, sizeof(s[i].log_message), "%s", timestramped_msg);
    i=(i+1)%sizeof(max_size);
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
