#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/mutex.h>

#define DEVICE_NAME "logger_driver"
#define MAX_LOGS 5
#define LOG_SIZE 128

static int major_number;
static char logs[MAX_LOGS][LOG_SIZE];
static int log_count = 0;
static int read_index = 0;
static int write_index = 0;
static struct mutex logger_mutex;

// Write operation
static ssize_t logger_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
    char log_entry[LOG_SIZE];
    struct timespec64 ts;

    if (len > LOG_SIZE - 30) { // Reserve space for timestamp
        pr_err("Log message too long\n");
        return -EINVAL;
    }

    if (copy_from_user(log_entry, buffer, len)) {
        pr_err("Failed to copy log message from user space\n");
        return -EFAULT;
    }

    log_entry[len] = '\0';
    ktime_get_real_ts64(&ts);

    mutex_lock(&logger_mutex);

    snprintf(logs[write_index], LOG_SIZE, "[%lld.%09lld] %s", ts.tv_sec, ts.tv_nsec, log_entry);
    write_index = (write_index + 1) % MAX_LOGS;

    if (log_count < MAX_LOGS)
        log_count++;
    else
        read_index = (read_index + 1) % MAX_LOGS;

    mutex_unlock(&logger_mutex);

    return len;
}

// Read operation
static ssize_t logger_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    size_t log_len;

    if (log_count == 0) {
        pr_info("No logs to read\n");
        return 0;
    }

    mutex_lock(&logger_mutex);

    log_len = strnlen(logs[read_index], LOG_SIZE);

    if (len < log_len) {
        pr_err("Buffer too small\n");
        mutex_unlock(&logger_mutex);
        return -EINVAL;
    }

    if (copy_to_user(buffer, logs[read_index], log_len)) {
        pr_err("Failed to copy log to user space\n");
        mutex_unlock(&logger_mutex);
        return -EFAULT;
    }

    read_index = (read_index + 1) % MAX_LOGS;
    log_count--;

    mutex_unlock(&logger_mutex);

    return log_len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = logger_write,
    .read = logger_read,
};

static int __init logger_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("Failed to register char device\n");
        return major_number;
    }

    pr_info("Logger device driver loaded with major number %d\n", major_number);
    mutex_init(&logger_mutex);

    return 0;
}

static void __exit logger_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("Logger device driver unloaded\n");
}

module_init(logger_driver_init);
module_exit(logger_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajini");
MODULE_DESCRIPTION("A simple logger device driver");

