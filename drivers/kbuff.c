#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024
#define DEVICE_NAME "simple_device"

static int open_count=0;
static int major_number;
static char device_buffer[BUFFER_SIZE];

static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode,struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file,const char __user *user_buffer,size_t size, loff_t *offset);

static struct file_operations fops={
	.open=device_open,
	.release=device_close,
	.read=device_read,
	.write=device_write,
};

static int device_open(struct inode *inode, struct file *file)
{
	open_count++;
	printk(KERN_INFO "simple_device: device opened %d times\n",open_count);
	return 0;
}

static int device_close(struct inode *inode, struct file *file)
{
	open_count--;
	printk(KERN_INFO "simple_device: device closed %d time\n",open_count);
	return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
	size_t len=strlen(device_buffer);
	if(*offset>=len)
		return 0;
	if(size>len-*offset)
		size=len-*offset;
	if(copy_to_user(user_buffer,device_buffer,size))
		return EFAULT;
	*offset+=size;
	printk(KERN_INFO "simple_device: sent %zu bytes to user\n",size);
	return size;
}

static ssize_t device_write(struct file *file,const char __user *user_buffer, size_t size, loff_t *offset)
{
	if(size>BUFFER_SIZE-1)
		size=BUFFER_SIZE -1;
	if(copy_from_user(device_buffer,user_buffer,size))
		return EFAULT;
	device_buffer[size]='\0';
	printk(KERN_INFO "simple_device: receives %zu bytes form the user\n",size);
	return size;
}

static int __init hello_init(void)
{
	major_number=register_chrdev(0,DEVICE_NAME,&fops);
	if(major_number<0)
	{
		printk(KERN_ALERT "simple_device: failed to register device\n");
		return major_number;
	}
	printk(KERN_INFO "simple_device:registered with major number %d\n",major_number);
	return 0;
}

static void __exit hello_exit(void)
{
	unregister_chrdev(major_number,DEVICE_NAME);
	printk(KERN_INFO "simple_device: unregistered device \n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("SUJITHA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple driver");
