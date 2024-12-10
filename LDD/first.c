#include <linux/module.h>
#include <linux/kernel.h>

//module initiation function
static int __init basic_driver_init(void){
        printk(KERN_INFO "Basic Driver Loaded\n");
        return 0;//return 0 indicates syccessful loading
}

//module cleanup function
static void __exit basic_driver_exit(void){
        printk(KERN_INFO "Basic Driver Loaded\n");

}

//Register module initiation and cleanup function
module_init(basic_driver_init);
module_exit(basic_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Meghana");
MODULE_DESCRIPTION("A Basic Linus Device Driver")
