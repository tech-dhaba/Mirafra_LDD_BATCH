#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define DEVICE_NAME "tmpfs_driver"
#define TMPFS_FILE_PATH "/tmp/tmpfs_example_file"
#define BUF_SIZE 256

static dev_t dev_num;
static struct cdev tmpfs_cdev;
static struct class *tmpfs_class;

// Buffer for temporary data
static char *kernel_buffer;

// File Operations
static ssize_t tmpfs_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    struct file *tmp_file;
    mm_segment_t old_fs;
    ssize_t bytes_read;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    tmp_file = filp_open(TMPFS_FILE_PATH, O_RDONLY, 0);
    if (IS_ERR(tmp_file)) {
        pr_err("Error opening tmpfs file\n");
        set_fs(old_fs);
        return PTR_ERR(tmp_file);
    }

    bytes_read = kernel_read(tmp_file, kernel_buffer, count, &tmp_file->f_pos);
    filp_close(tmp_file, NULL);
    set_fs(old_fs);

    if (copy_to_user(buf, kernel_buffer, bytes_read))
        return -EFAULT;

    pr_info("Read %zd bytes from tmpfs file\n", bytes_read);
    return bytes_read;
}

static ssize_t tmpfs_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    struct file *tmp_file;
    mm_segment_t old_fs;
    ssize_t bytes_written;

    if (copy_from_user(kernel_buffer, buf, count))
        return -EFAULT;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    tmp_file = filp_open(TMPFS_FILE_PATH, O_WRONLY | O_CREAT, 0644);
    if (IS_ERR(tmp_file)) {
        pr_err("Error opening tmpfs file for writing\n");
        set_fs(old_fs);
        return PTR_ERR(tmp_file);
    }

    bytes_written = kernel_write(tmp_file, kernel_buffer, count, &tmp_file->f_pos);
    filp_close(tmp_file, NULL);
    set_fs(old_fs);

    pr_info("Wrote %zd bytes to tmpfs file\n", bytes_written);
    return bytes_written;
}

static int tmpfs_open(struct inode *inode, struct file *file) {
    pr_info("tmpfs_driver opened\n");
    return 0;
}

static int tmpfs_release(struct inode *inode, struct file *file) {
    pr_info("tmpfs_driver closed\n");
    return 0;
}

// File operations structure
static struct file_operations tmpfs_fops = {
    .owner = THIS_MODULE,
    .read = tmpfs_read,
    .write = tmpfs_write,
    .open = tmpfs_open,
    .release = tmpfs_release,
};

// Module init function
static int __init tmpfs_driver_init(void) {
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate major number\n");
        return -1;
    }

    cdev_init(&tmpfs_cdev, &tmpfs_fops);
    if (cdev_add(&tmpfs_cdev, dev_num, 1) < 0) {
        unregister_chrdev_region(dev_num, 1);
        pr_err("Failed to add cdev\n");
        return -1;
    }

    tmpfs_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(tmpfs_class)) {
        cdev_del(&tmpfs_cdev);
        unregister_chrdev_region(dev_num, 1);
        pr_err("Failed to create class\n");
        return PTR_ERR(tmpfs_class);
    }

    device_create(tmpfs_class, NULL, dev_num, NULL, DEVICE_NAME);

    kernel_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        device_destroy(tmpfs_class, dev_num);
        class_destroy(tmpfs_class);
        cdev_del(&tmpfs_cdev);
        unregister_chrdev_region(dev_num, 1);
        return -ENOMEM;
    }

    pr_info("tmpfs_driver loaded\n");
    return 0;
}

// Module exit function
static void __exit tmpfs_driver_exit(void) {
    kfree(kernel_buffer);
    device_destroy(tmpfs_class, dev_num);
    class_destroy(tmpfs_class);
    cdev_del(&tmpfs_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("tmpfs_driver unloaded\n");
}

module_init(tmpfs_driver_init);
module_exit(tmpfs_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Raspberry Pi tmpfs Driver Example");
