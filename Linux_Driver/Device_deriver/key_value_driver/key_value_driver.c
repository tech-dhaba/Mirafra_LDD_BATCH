/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

static int major_number;
static int count=10;
static int i=0,j=0;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
struct key
{
	char key[64];
	char value[256];
};
static struct key s[10];
static char message[]="limit exceed";
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
   // size_t len = strlen(device_buffer);
	char read_buff[100];
	if(j<count)
	{	
		snprintf(read_buff, sizeof(read_buff), "%s %s",s[j].key,s[j].value);
		printk(KERN_INFO "read_buf:%s",read_buff);
		j++;
   		 if (copy_to_user(user_buffer,read_buff,strlen(read_buff)+1))
   		 {
   		     return -EFAULT;
   		 }
		printk(KERN_INFO "user buffer:%s",user_buffer);

	}
	else if(j==count)
	{
		j++;
		 if (copy_to_user(user_buffer,device_buffer,strlen(device_buffer)+1))
   		 {
   		     return -EFAULT;
   		 }
	}
	if(j>count)
	{	
	//	snprintf(device_buffer, sizeof(device_buffer), "%s","nothing to read");
		if (copy_to_user(user_buffer,device_buffer,strlen(device_buffer)+1))
   		 {
   		     return -EFAULT;
   		 }
	}



    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
   // if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
     //   size = BUFFER_SIZE - 1;
	int k=0;
	char key[63];
	char buffer[322];
	if(count>i)
	{
    	if (copy_from_user(&s[i], user_buffer, sizeof(struct key))) {
        return -EFAULT;
    	}
		printk(KERN_INFO "%s is key and %s is my value\n",s[i].key,s[i].value);
		i++;
	}
	else if(count==i)
	{
		i++;
		snprintf(device_buffer, sizeof(device_buffer), "%s", message);
		device_buffer[strlen(message)] = '\0';
		printk(KERN_INFO "THE device buffer is %s",device_buffer);
	}
	else
	{
		if (copy_from_user(key, user_buffer, size)) return -EFAULT;
        	key[size] = '\0';
        	for ( k = 0; k < count; k++)
		   	{
            	if (strcmp(key, s[k].key) == 0) 
				{
               		 snprintf(buffer, sizeof(buffer), "%s %s", s[k].key, s[k].value);
            	   	 snprintf(device_buffer, sizeof(device_buffer), "%s", buffer);
               		 printk(KERN_INFO "Found: %s\n", buffer);
               		 return size;
            	}
        	}
        snprintf(device_buffer, sizeof(device_buffer), "Key not found");
        printk(KERN_INFO "Key not found\n");	
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



