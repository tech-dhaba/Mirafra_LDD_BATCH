#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
//#include<linux/slab.h>
//#include<linux/kmalloc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VAJID");
MODULE_DESCRIPTION("A SIMPLE DRIVER");
#define DEVICE_NAME "Simple_driver"
static int major;

static int device_open(struct inode *,struct file *);
static int device_close(struct inode *,struct file *);
static ssize_t device_read(struct file * ,char __user *,size_t,loff_t *);
static ssize_t device_write(struct file *file,const char __user *,size_t,loff_t *);


static struct file_operations fops=
{
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close,
};


static int device_open(struct inode *inode,struct file *file)
{
	pr_info("Device opened sucessfully");
	return 0;
}

static int device_close(struct inode *inode,struct file *file)
{
	pr_info("device closed sucessfully");
	return 0;
}

static ssize_t device_read(struct file *file,char __user *buffer,size_t size,loff_t *offset)
{
	char str[100];
	printk(KERN_INFO "device is reading");
	if(copy_from_user(str,buffer,size))
	{
		printk(KERN_ALERT "copy_from_user is not done");
		return -EFAULT;
	}
	printk(KERN_INFO "Read from user :%s",str);
	return 0;
}
static ssize_t device_write(struct file *file,const char __user *buffer,size_t size,loff_t *offset)
{
//	char str[100];
	pr_info("Writing to user buffer");
//	strcpy(buffer,"HI KERNEL WRITTEN SUCESSFULLY");
	pr_info("Data written sucessfully");
	return 0;
}

static int __init init_simple_driver(void)
{
	major = register_chrdev(0,DEVICE_NAME,&fops);
	if(major<0)
	{
		pr_err(KERN_ALERT "Module initialization failed");
		return -EFAULT;
	}
	printk(KERN_INFO "Module initialized sucessfully with major nomber %d",major);
	return major;
}
static void __exit exit_simple_driver(void)
{
	pr_info(KERN_INFO "Module unloaded sucessfully");
}

module_init(init_simple_driver);
module_exit(exit_simple_driver);
