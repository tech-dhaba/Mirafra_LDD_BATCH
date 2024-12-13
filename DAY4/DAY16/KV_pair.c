#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "KEY_VALUE_STORE"
#define MAX_KEY_LEN 20
#define MAX_VAL_LEN 10
#define MAX_KEY_VAL_PAIR 10

struct kv_store{
	char key[MAX_KEY_LEN];
	char val[MAX_VAL_LEN];
};

struct kv_store KV_STORE[MAX_KEY_VAL_PAIR];
static int key_val_count;
static int major_number;

//static int  kv_open(struct file *file,struct inode *inode);
static ssize_t kv_read(struct file *file,char __user *buffer,size_t len,loff_t *offset);
static ssize_t kv_write(struct file *file,const char __user *buffer,size_t len,loff_t *offset);
//static void kv_close(struct file *file ,struct file *inode);

static struct file_operations fops={
//	.open = kv_open;
	.read = kv_read,
	.write = kv_write,
//	.close = kv_close;
};

/*static int kv_open(struct file *file ,struct inode *inode)
{
	printk(KERN_INFO "file opened sucessfully\n");
	return 0;
}

static int kv_close(struct file *file,struct inode *inode)
{
	printk(KERN_INFO "file closed sucessfully");
	return 0;
}
*/
static ssize_t kv_write(struct file *file,const char __user *buffer,size_t len,loff_t *offset)
{
	char *key=NULL,*val=NULL,*kbuf=NULL;
	if(len >( MAX_KEY_LEN + MAX_VAL_LEN +2))
	{
		printk(KERN_INFO "Buffer size overflow");
		return -EINVAL;
	}
	kbuf=(char *)kmalloc(MAX_KEY_LEN+MAX_VAL_LEN +2,GFP_KERNEL);
	if(!kbuf)
	{
		pr_err("Memory not allocated to kbuf\n");
		return -ENOMEM;
	}
	kbuf[len]='\0';
	if(copy_from_user(kbuf,buffer,len))
	{
		kfree(kbuf);
		pr_err("not able to copy in kbuf\n");
		return -EFAULT;
	}

	key=strsep(&kbuf,",");
	val=kbuf;

	if(!key || !val ||(strlen(key)> MAX_KEY_LEN) || (strlen(val)>MAX_VAL_LEN))
	{
		kfree(kbuf);
		return -EFAULT;
	}

	if(key_val_count>=MAX_KEY_VAL_PAIR)
	{
		printk(KERN_INFO "Reached to MAXX\n");
		kfree(kbuf);
		return -EFAULT;
	}

	strncpy(KV_STORE[key_val_count].key,key,strlen(key)+1);
	strncpy(KV_STORE[key_val_count].val,val,strlen(val)+1);
	key_val_count++;

	kfree(kbuf);
	return len;
}

static ssize_t kv_read(struct file *file,char __user *buffer,size_t len,loff_t *offset)
{
	char *key;
	int i=0;
	char result[MAX_KEY_LEN];
	
	key = (char *) kmalloc(MAX_KEY_LEN , GFP_KERNEL);
	if(!key)
	{
		printk("Memory not allocated to read key");
		return -EFAULT;
	}

	if(copy_from_user(key,buffer,len))
	{
		printk(KERN_INFO "Not copied to kernel buffer\n");
		kfree(key);
		return -EFAULT;
	}

	for(i=0;i<key_val_count;i++)
	{
		if(strcmp(key,KV_STORE[i].key)==0)
		{
			strncpy(result,KV_STORE[i].val,len);
			kfree(key);
			if(copy_to_user(buffer,result,MAX_VAL_LEN))
			{
				printk(KERN_ALERT "not copied result to user buffer");
				return -EFAULT;
			}
			return strlen(result);
		}
	}

		kfree(key);
		/*if(copy_to_user(buffer,"key not there",100))
		{
			printk(KERN_INFO "KEY not present in list\n");
		}
		*/
		strcpy(buffer,"key not present in list");
		return -ENOENT;
}

static int __init kv_store_init(void)
{
	printk(KERN_INFO "Initializing KV_module");
	major_number = register_chrdev(0,DEVICE_NAME,&fops);
	if(major_number<0)
	{
		printk("module not loaded\n");
	}
	else printk(KERN_INFO "KV_module loaded with major:%d\n",major_number);
	return 0;
}

static void __exit kv_store_exit(void)
{
	printk(KERN_INFO "module unloaded KV_module");
}

module_init(kv_store_init);
module_exit(kv_store_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VAJID PINJARI");
MODULE_DESCRIPTION("KV DRIVER");
