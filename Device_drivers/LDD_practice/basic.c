#include<linux/module.h>
#include<linux/kernel.h>

static int __init simple_driver_init(void)
{
	printk(KERN_INFO "Basic driver code loaded");
	return 0;
}

static void __exit simple_driver_exit(void)
{
  printk(KERN_INFO "driver code unloaded");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("balasai");
MODULE_DESCRIPTION("a basic driver code");
