#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/slab.h>
/* device name defining */
#define DEVICE_NAME "logger_driver"
#define BUFFER_SIZE 5
#define LOG_MSG_SIZE 256

static int major_number;
static char logs[BUFFER_SIZE][LOG_MSG_SIZE];
static int head = 0;
static int tail = 0;
static int count = 0;
static struct mutex log_lock;

/* function  Prototypes declared */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);

/*  File operations structure that points to function to perforn the operations */
static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .write = device_write,
    .read = device_read,
};

/* the function for  Open device */
static int device_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "logger_driver: Device opened\n");
    return 0;
}

/* THE FUNCTION for  Close device */
static int device_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "logger_driver: Device closed\n");
    return 0;
}

/* Write log to the circular buffer   */
static ssize_t device_write(struct file *filep, const char __user *user_buffer, size_t len, loff_t *offset) {
    char temp_buffer[LOG_MSG_SIZE];
    struct timespec64 ts;
    char timestamp[64];

    if (len >= LOG_MSG_SIZE - 64) {
        printk(KERN_WARNING "logger_driver: Log message too long\n");
        return -EINVAL;
    }

    if (copy_from_user(temp_buffer, user_buffer, len)) {
        return -EFAULT;
    }
    temp_buffer[len] = '\0';

    /*  Get the current time  */
    ktime_get_real_ts64(&ts);
    snprintf(timestamp, sizeof(timestamp), "[%lld.%06ld] ", ts.tv_sec, ts.tv_nsec / 1000);

    /* Write log with timestamp  */
    mutex_lock(&log_lock);
    snprintf(logs[head], LOG_MSG_SIZE, "%s%s", timestamp, temp_buffer);
    head = (head + 1) % BUFFER_SIZE;
    if (count < BUFFER_SIZE) {
        count++;
    } else {
        tail = (tail + 1) % BUFFER_SIZE;  // Overwrite the oldest log
    }
    mutex_unlock(&log_lock);

    printk(KERN_INFO "logger_driver: Log written: %s\n", logs[head ? head - 1 : BUFFER_SIZE - 1]);
    return len;
}

/*  Read logs from the circular buffer   */
static ssize_t device_read(struct file *filep, char __user *user_buffer, size_t len, loff_t *offset) {
    int log_len;

    mutex_lock(&log_lock);

    if (count == 0) {
        mutex_unlock(&log_lock);
        printk(KERN_INFO "logger_driver: No logs available\n");
        return 0;
    }

    log_len = strlen(logs[tail]);
    if (log_len > len) {
        mutex_unlock(&log_lock);
        printk(KERN_WARNING "logger_driver: Buffer size too small\n");
        return -EINVAL;
    }

    if (copy_to_user(user_buffer, logs[tail], log_len)) {
        mutex_unlock(&log_lock);
        return -EFAULT;
    }

    tail = (tail + 1) % BUFFER_SIZE;
    count--;
    mutex_unlock(&log_lock);

    printk(KERN_INFO "logger_driver: Log read\n");
    return log_len;
}

/*  Initialize the module   */
static int __init logger_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "logger_driver: Failed to register device\n");
        return major_number;
    }

    mutex_init(&log_lock);

    printk(KERN_INFO "logger_driver: Registered with major number %d\n", major_number);
    return 0;
}

/*  Exit the module   */
static void __exit logger_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    mutex_destroy(&log_lock);
    printk(KERN_INFO "logger_driver: Unregistered device\n");
}

module_init(logger_driver_init);
module_exit(logger_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth reddy");
MODULE_DESCRIPTION("Logger Device Driver with Circular Buffer");

