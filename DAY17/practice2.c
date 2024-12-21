#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define DEVICE_NAME "Simple Driver"
static int major;
static struct file_operations fops={};
static int __init mymodule_init(void)
{
	printk("Initialinig simple module");
	major= register_chrdev(0,DEVICE_NAME,&fops);
	if(major<0)
	{
		printk("Module loading failed");
		return -EFAULT;
	}
	printk("MODULE loaded in kernel with major number %d\n",major);
	return major;
}
static void __exit mymodule_exit(void)
{
	unregister_chrdev(major,DEVICE_NAME);
	printk("Module unloaded");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VAJID");
MODULE_DESCRIPTION("SIMPLE DRIVER");
