//learinig kmalloc and vmalloc

#include<linux/kernel.h> //for printk
#include<linux/module.h> //for module_init & module_exit
#include<linux/slab.h>    //for kmalloc
#include<linux/vmalloc.h>  //for vmalloc
#include<linux/fs.h>    //for character device driver


#define KMALLOC_SIZE 1024
#define VMALLOC_SIZE 1024

static void *kmalloc_memory = NULL;
static void *vmalloc_memory = NULL;

static int __init init_alloc_dyn_mem(void)
{
	printk(KERN_INFO "Initializing kernel memory\n");
	kmalloc_memory =(char*) kmalloc((sizeof(char))*KMALLOC_SIZE ,GFP_KERNEL);
	if(!kmalloc_memory)
	{
		pr_err("unable to allocate memory in kernel by kmalloc\n");
		return -ENOMEM;
	}
	printk(KERN_INFO "Allocated memory by malloc\n");
	vmalloc_memory= (char*) vmalloc((sizeof(char)*VMALLOC_SIZE,GFP_KERNEL));
	if(!vmalloc_memory)
	{
		kfree(kmalloc_memory);
		pr_err("unable to allocate memory in kernel by vmalloc\n");
		return -ENOMEM;
	}
	printk(KERN_INFO "Allocated memory by vmalloc\n");
	return 0;
}

static void __exit exit_alloc_dyn_mem(void)
{
	printk(KERN_INFO "Deallocating kernel memory by kmalloc\n");
	if(kmalloc_memory)
	{
		kfree(kmalloc_memory);
	}
	if(vmalloc_memory)
	{
	printk(KERN_INFO "Deallocating kernel memory by vmalloc\n");
	vfree(vmalloc_memory);
	}
}

//module init and exit 
module_init(init_alloc_dyn_mem);
module_exit(exit_alloc_dyn_mem);

//Driver description
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VAJID");
MODULE_DESCRIPTION("Allocating Dynamic memory by kmalloc & vmalloc\n");
