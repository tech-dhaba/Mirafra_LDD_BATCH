#include<linux/module.h> //used for "module_inii", "module_exit" and other module functionalities
#include<linux/kernel.h> //provide access to kernel-specific functions and macros (like printk)

//module initialization function
static int __init hello_init(void)
{
	printk(KERN_INFO "basic driver loaded\n");
	return 0; //return 0 indicates successful loading
}

//module cleanup functio
static void __exit hello_exit(void)
{
	printk(KERN_INFO "basic driver unloaded\n");
}

//register module initialization and cleanup functions
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sujitha");
MODULE_DESCRIPTION("a basic linux device driver");

