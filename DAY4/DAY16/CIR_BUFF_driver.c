#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>

#define DEVICE_NAME "logger_driver"
#define MAX_LOGS 5
#define MAX_LOG_LEN 256

struct log_entry {
    char message[MAX_LOG_LEN];
    char timestamp[32];
};

static struct log_entry log_buffer[MAX_LOGS];
static int log_start = 0; // Start index of the circular buffer
static int log_count = 0; // Number of logs currently in the buffer
static int major_number;

static int logger_open(struct inode *inode, struct file *file);
static int logger_release(struct inode *inode, struct file *file);
static ssize_t logger_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset);
static ssize_t logger_read(struct file *file, char __user *buffer, size_t len, loff_t *offset);

static struct file_operations fops = {
    .open = logger_open,
    .release = logger_release,
    .write = logger_write,
    .read = logger_read,
};

static void get_simple_timestamp(char *timestamp, size_t size) {
    struct timespec64 ts;
    ktime_get_real_ts64(&ts);

    // Convert seconds since epoch to string
    long long seconds = ts.tv_sec;
    char buffer[32];
    int i = 0;

    if (seconds == 0) {
        timestamp[i++] = '0';
    } else {
        while (seconds > 0 && i < size - 1) {
            buffer[i++] = (seconds % 10) + '0';
            seconds /= 10;
        }

        for (int j = 0; j < i; j++) {
            timestamp[j] = buffer[i - j - 1];
        }
    }

    timestamp[i] = '\0';
}

static int logger_open(struct inode *inode, struct file *file) {
    pr_info("logger_driver: device opened\n");
    return 0;
}

static int logger_release(struct inode *inode, struct file *file) {
    pr_info("logger_driver: device closed\n");
    return 0;
}

static ssize_t logger_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
    char kbuf[MAX_LOG_LEN];

    if (len > MAX_LOG_LEN - 1)
        return -EINVAL;

    if (copy_from_user(kbuf, buffer, len))
        return -EFAULT;

    kbuf[len] = '\0';

    // Append log message and simple timestamp to the buffer
    get_simple_timestamp(log_buffer[(log_start + log_count) % MAX_LOGS].timestamp, sizeof(log_buffer[0].timestamp));
    strncpy(log_buffer[(log_start + log_count) % MAX_LOGS].message, kbuf, MAX_LOG_LEN);

    if (log_count < MAX_LOGS) {
        log_count++;
    } else {
        log_start = (log_start + 1) % MAX_LOGS; // Overwrite oldest log
    }

    return len;
}

static ssize_t logger_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    char log_entry[MAX_LOG_LEN + 32];

    if (*offset != 0)
        return 0; // EOF

    if (log_count == 0) {
        return -ENODATA; // No logs available
    }

    snprintf(log_entry, sizeof(log_entry), "[%s] %s\n",
             log_buffer[log_start].timestamp,
             log_buffer[log_start].message);

    log_start = (log_start + 1) % MAX_LOGS;
    log_count--;

    if (len < strlen(log_entry))
        return -EINVAL;

    if (copy_to_user(buffer, log_entry, strlen(log_entry)))
        return -EFAULT;

    *offset += strlen(log_entry);

    return strlen(log_entry);
}

static int __init logger_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("Failed to register device\n");
        return major_number;
    }

    pr_info("logger_driver registered with major number %d\n", major_number);
    return 0;
}

static void __exit logger_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("logger_driver unregistered\n");
}

module_init(logger_init);
module_exit(logger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vajid");
MODULE_DESCRIPTION("Logger Device Driver with Circular Buffer and Simple Timestamp");
MODULE_VERSION("1.0");
