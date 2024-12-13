#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define NO_OF_DEVICES 4

#define BUFFER_SIZE_PCDEV1 512
#define BUFFER_SIZE_PCDEV2 1024
#define BUFFER_SIZE_PCDEV3 512
#define BUFFER_SIZE_PCDEV4 1024

#define RDONLY 0100
#define WRONLY 0010
#define RDWR  0110


static int __init pcd_driver_init(void);
static void __exit pcd_driver_exit(void);

static int pcd_open (struct inode * inode, struct file * file);
static int pcd_close (struct inode * inode, struct file *file);
static ssize_t pcd_read (struct file *filp,  char __user *buff, size_t size,loff_t *offset);
static ssize_t pcd_write (struct file *filp, const char __user *buff, size_t size,loff_t *offset);

//PCDEV MEMORY
static char device_buffer_pcdev1[BUFFER_SIZE_PCDEV1];
static char device_buffer_pcdev2[BUFFER_SIZE_PCDEV2];
static char device_buffer_pcdev3[BUFFER_SIZE_PCDEV3];
static char device_buffer_pcdev4[BUFFER_SIZE_PCDEV4];


//static int open_count = 0;


static struct file_operations pcd_fops = 
{
	.owner = THIS_MODULE,
	.open = pcd_open,
	.release = pcd_close,
	.read = pcd_read,
	.write = pcd_write
};

 struct pcdev_private_data
{
	char *buffer;
	const char *serial_number;
	unsigned size;
	int perm;
	struct cdev cdev;
	
};
struct pcdrv_private_data
{
	int total_devices;
    dev_t device_number;       
	struct class *class_pcd;
	struct device *device_pcd;
	struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

static struct pcdrv_private_data pcdrv_data =
{
	.total_devices = NO_OF_DEVICES,
	.pcdev_data={
				[0]={
					.buffer = device_buffer_pcdev1,
					.size = BUFFER_SIZE_PCDEV1,
					.serial_number = "PCDEV1",
					.perm = 0110
					},

				[1]={
					.buffer = device_buffer_pcdev2,
					.size = BUFFER_SIZE_PCDEV2,
					.serial_number = "PCDEV2",
					.perm = 0110
					},
				
				[2]={
					.buffer = device_buffer_pcdev3,
					.size = BUFFER_SIZE_PCDEV3,
					.serial_number = "PCDEV3",
					.perm = 0100
					},
				
				[3]={
					.buffer = device_buffer_pcdev4,
					.size = BUFFER_SIZE_PCDEV4,
					.serial_number = "PCDEV4",
					.perm = 0100
					}
				}
};

static int __init pcd_driver_init(void)
{
	int i;

    // Allocate device number
    if ((alloc_chrdev_region(&pcdrv_data.device_number, 0, NO_OF_DEVICES, "pcddev")) < 0) {
        pr_err("Cannot allocate major number\n");
        return -1;
    }

    // Create struct class
    if (IS_ERR( pcdrv_data.class_pcd = class_create(THIS_MODULE,"pcd_class"))) {
        pr_err("Cannot create the struct class\n");
        goto r_unregister;
    }

	for(i=0 ; i<NO_OF_DEVICES; i++) {
    	pr_info("Device_number <Major:Minor> = %d:%d\n", MAJOR(pcdrv_data.device_number+i), MINOR(pcdrv_data.device_number+i));
	
    // Create cdev structure
    cdev_init(&pcdrv_data.pcdev_data[i].cdev, &pcd_fops);

    // Add character device to the system
    if ((cdev_add(&pcdrv_data.pcdev_data[i].cdev,pcdrv_data.device_number + i, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto r_class_destroy;
    }



    // Create device
    if (IS_ERR(device_create(pcdrv_data.class_pcd, NULL, pcdrv_data.device_number + i, NULL, "pcdev-%d",i+1))) {
        pr_err("Cannot create the Device\n");
        goto r_class_destroy;
    }
}

    pr_info("Module Loaded\n");
    return 0;

r_class_destroy:
	for(;i>=0;i--){
	device_destroy(pcdrv_data.class_pcd, pcdrv_data.device_number+i);
	cdev_del(&pcdrv_data.pcdev_data[i].cdev);
}
    class_destroy(pcdrv_data.class_pcd);
r_unregister:
    unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);
    return -1;

}

static void __exit pcd_driver_exit(void)
{
	int i;
	for(i=0;i<NO_OF_DEVICES;i++){
	device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_number+i);
	cdev_del(&pcdrv_data.pcdev_data[i].cdev);
	}
	class_destroy(pcdrv_data.class_pcd);
	unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);
	pr_info("Module unloaded\n");
	
}
// Called when the device is opened

static int check_permission(int dev_perm,int acc_mode)
{
	if(dev_perm ==RDWR)
		return 0;
	
	if( (dev_perm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
		return 0;

	if((dev_perm == WRONLY ) && ((acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ)))
		return 0;

	return -EPERM;

//	return 0;
}

static int pcd_open(struct inode *inode, struct file *file) {
    int ret ;
	int minor_n;
	
	struct pcdev_private_data *pcdev_data;

	minor_n = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n",minor_n);

	pcdev_data = container_of(inode->i_cdev,struct pcdev_private_data,cdev);

	file->private_data = pcdev_data;

	ret = check_permission(pcdev_data->perm,file->f_mode);

	(!ret)?pr_info("open was successful\n"):pr_info("open was unsuccessful\n");

	return ret;
}

// Called when the device is closed
static int pcd_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}



// Called when data is read from the device
static ssize_t pcd_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset) {


	struct pcdev_private_data *pcdev_data =(struct pcdev_private_data*)file->private_data;


    size_t len = strlen(pcdev_data->buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (count > len - *offset) // Adjust size to avoid reading beyond buffer
        count = len - *offset;
	
    if (copy_to_user(user_buffer, pcdev_data->buffer + *offset, count)) {
        return -EFAULT;
    }

    *offset += count;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", count);
    return count;

	return 0;
}

// Called when data is written to the device
static ssize_t pcd_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset) {

	struct pcdev_private_data *pcdev_data =(struct pcdev_private_data*)file->private_data;


    if (count + *offset  > pcdev_data->size ) // Limit size to buffer capacity
        count = pcdev_data->size - *offset;

    if (copy_from_user(pcdev_data->buffer + *offset, user_buffer, count)) {
        return -EFAULT;
    }

	*offset += count;
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", sizeof(pcdev_data->buffer));
	return count;

return 0;
}
module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KGCSR");
MODULE_DESCRIPTION("PCD with Multiple Nodes");
MODULE_VERSION("1.0");
