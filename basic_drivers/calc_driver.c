#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"   // giving a name for the device and defining a macro
#define BUFFER_SIZE 1024  //defining a macro with size 1024 

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

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
    size_t len = strlen(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }
//      *user_buffer=*user_buffer*2;

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
	if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
		size = BUFFER_SIZE - 1;

	if (copy_from_user(device_buffer, user_buffer, size)) {
		return -EFAULT;
	}

	device_buffer[size] = '\0'; // Null-terminate the string
	printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
	int num1=0,num2=0,num3=0,i,j;
	char t,op;
	for(i=0;device_buffer[i];i++)
	{
		if(device_buffer[i]>='0' && device_buffer[i]<='9')
		{
			num1=(num1*10)+(device_buffer[i]-48);
		}
		else
			break;
	}
	op=device_buffer[i];
	for(i=i+1;device_buffer[i];i++)
	{
		if(device_buffer[i]>='0' && device_buffer[i]<='9')
			num2=(num2*10)+(device_buffer[i]-48);
		else
			break;
	}
	if(op=='+')
		num3=num1+num2;
	else if(op=='-')
		num3=num1-num2;
	else if(op=='*')
		num3=num1*num2;
	else if(op=='/')
		num3=num1/num2;
	for(i=0;num3;i++,num3=num3/10)
	{
		device_buffer[i]=(num3%10)+48;
	}
	device_buffer[i]='\0';
	for(j=i-1,i=0;i<j;i++,j--)
	{
		t=device_buffer[i];
		device_buffer[i]=device_buffer[j];
		device_buffer[j]=t;

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
MODULE_AUTHOR("AVINASH");
MODULE_DESCRIPTION("A Simple Linux Device Driver");
