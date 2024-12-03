#include<linux/module.h>
#include<linux/kernel.h>
//module initialization function
static int __init basic_driver_init(void)
{
	printk(KERN_INFO "basic driver loaded\n");
	return 0;//return 0 indicates successful loading
		 //}

	}		 //module cleanup function
static void __exit basic_driver_exit(void)
{
	printk(KERN_INFO "basic driver unloaded\n");
}
//register module initialization and cleanup functions
module_init(basic_driver_init);
module_exit(basic_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajini");
MODULE_DESCRIPTION("A basic linus device driver");

           
