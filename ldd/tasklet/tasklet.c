#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Enhanced Tasklet Driver Example");

/* Device and IOCTL Definitions */
#define DEVICE_NAME "tasklet_dev"
#define IOCTL_TRIGGER_TASKLET _IO('T', 1)  // IOCTL to trigger tasklet

/* Tasklet Declaration */
static struct tasklet_struct my_tasklet;

/* Tasklet Function */
static void tasklet_function(unsigned long data)
{
    pr_info("Tasklet executed! Data: %ld\n", data);
}

/* File Operations */
/* Open Device */
static int dev_open(struct inode *inodep, struct file *filep)
{
    pr_info("Device %s opened\n", DEVICE_NAME);
    return 0;
}

/* Release Device */
static int dev_release(struct inode *inodep, struct file *filep)
{
    pr_info("Device %s closed\n", DEVICE_NAME);
    return 0;
}

/* IOCTL Function */
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case IOCTL_TRIGGER_TASKLET:
            pr_info("Tasklet triggered via IOCTL\n");
            tasklet_schedule(&my_tasklet);  // Schedule the tasklet
            break;
        default:
            pr_err("Invalid IOCTL command\n");
            return -EINVAL;
    }
    return 0;
}

/* File Operations Structure */
static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .unlocked_ioctl = dev_ioctl,
};

/* Major Number for Device */
static int major;

/* Module Initialization */
static int __init tasklet_driver_init(void)
{
    //int ret;

    pr_info("Loading Tasklet Driver\n");

    /* Register Character Device */
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("Failed to register device: %d\n", major);
        return major;
    }
    pr_info("Device registered with major number %d\n", major);

    /* Initialize Tasklet */
    tasklet_init(&my_tasklet, tasklet_function, 1);  // Initial data = 1

    return 0;
}

/* Module Cleanup */
static void __exit tasklet_driver_exit(void)
{
    pr_info("Unloading Tasklet Driver\n");

    /* Kill the Tasklet */
    tasklet_kill(&my_tasklet);

    /* Unregister Character Device */
    unregister_chrdev(major, DEVICE_NAME);

    pr_info("Tasklet Driver Unloaded\n");
}

module_init(tasklet_driver_init);
module_exit(tasklet_driver_exit);
