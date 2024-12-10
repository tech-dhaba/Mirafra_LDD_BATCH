#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "kv_store"
#define BUFFER_SIZE 1024
#define MAX 4

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
static int count = 0;

struct key_value 
{
	char key[128];
	char value[128];
};

static struct key_value kv[MAX];

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

// Device open function
static int device_open(struct inode *inode, struct file *file) {
	open_count++;
	printk(KERN_INFO "kv_store: Device opened %d time(s)\n", open_count);
	return 0;
}

// Device close function
static int device_close(struct inode *inode, struct file *file) {
	open_count--;
	printk(KERN_INFO "kv_store: Device closed, remaining opens: %d\n", open_count);
	return 0;
}

// Device read function
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {

    if(count == MAX)
	{
    printk(KERN_INFO "ready to store user buffer");
	copy_to_user(user_buffer,device_buffer + *offset,size);
	printk(KERN_INFO "copy_to_user %s\n",user_buffer);
	}
	else if(count<MAX)
	{
	printk(KERN_INFO "ready to store user buffer");
	copy_to_user(user_buffer,device_buffer + *offset,size);
	printk(KERN_INFO "copy_to_user %s\n",user_buffer);
	//count++;
	}

else if(count>MAX)
	 printk(KERN_INFO "value updating ready");
	//else
	 {
		 copy_to_user(user_buffer,device_buffer,sizeof(device_buffer));
	 }

	printk(KERN_INFO "kv_store: Sent %zu bytes to the user\n", size);
	return size;
}

// Device write function
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {

		printk(KERN_INFO "values received key value");
    	char message[100]="limit exceed";
			if (count== MAX)
		{
			count++;
		snprintf(device_buffer, sizeof(device_buffer),"%s",message);
		device_buffer[strlen(message)]='\0';
		printk(KERN_ERR "Maximum key :%s\n",device_buffer);
		}
		else if(count<MAX)
		{
	    copy_from_user(&kv[count],user_buffer,sizeof(struct key_value));			
		snprintf(device_buffer,sizeof(device_buffer),"%s %s",kv[count].key,kv[count].value);
		printk(KERN_INFO "kv_store: Stored key-value pair: %s and  %s\n",kv[count].key,kv[count].value);
		printk(KERN_INFO "received user to device %s ",device_buffer);
		count++;
		}
		else
			{
		int i=0;
		char k[128];
		char buffer[128];
		copy_from_user(buffer,user_buffer,sizeof(buffer));
		  for(i=0;i<MAX;i++)
		  {     printk(KERN_INFO "entering to strcmp"); 
				if(strcmp(buffer,kv[i].key)==0)
				{
					//buffer[128]={0};
					sprintf(buffer,"%s",kv[i].value);
		    		snprintf(device_buffer,sizeof(device_buffer),"%s",buffer);
					printk(KERN_INFO "buffer updated");
					printk(KERN_INFO "%s",device_buffer);
					//device_buffer[strlen(buffer)]='\0';
					return size;
				}		
			}
	snprintf(device_buffer,sizeof(device_buffer),"key not found");
	printk(KERN_INFO "key not found\n");
			}
		return size;
}		  

// Module initialization
static int __init kv_store_init(void) {
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number < 0) {
		printk(KERN_ALERT "kv_store: Failed to register device\n");
		return major_number;
	}
	printk(KERN_INFO "kv_store: Registered with major number %d\n", major_number);
	return 0;
}

// Module cleanup
static void __exit kv_store_exit(void) {
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "kv_store: Unregistered device\n");
}

module_init(kv_store_init);
module_exit(kv_store_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Key-Value Store Character Device");
