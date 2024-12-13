#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "kv_store"
#define MAX_ENTRIES 5
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 32

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Meghana");
MODULE_DESCRIPTION("A simple key-value store kernel module");

static int major_number;
static struct kv_entry {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} kv_store[MAX_ENTRIES];

static int kv_count = 0;  // To track the number of stored key-value pairs

// Open function
static int kv_open(struct inode *inode, struct file *file) {
    return 0;
}

// Read function (Get value by key)
static ssize_t kv_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset) {
    char buffer[MAX_KEY_LEN + MAX_VALUE_LEN];
    char input_key[MAX_KEY_LEN];
    int i;

    if (kv_count == 0) {
        return 0;  // No key-value pairs to read
    }

    if (copy_from_user(input_key, user_buffer, len)) {
        return -EFAULT;  // Failed to copy key from user space
    }

    input_key[len] = '\0';  // Null-terminate the string

    // Search for the key and return the corresponding value
    for (i = 0; i < kv_count; i++) {
        if (strncmp(kv_store[i].key, input_key, MAX_KEY_LEN) == 0) {
            snprintf(buffer, sizeof(buffer), "%s=%s", kv_store[i].key, kv_store[i].value);
            if (copy_to_user(user_buffer, buffer, strlen(buffer) + 1)) {
                return -EFAULT;
            }
            return strlen(buffer);  // Return the number of bytes read
        }
    }

    // Key not found
    return -ENOENT;  // No such entry
}

// Write function (Store key-value pair)
static ssize_t kv_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset) {
    char buffer[MAX_KEY_LEN + MAX_VALUE_LEN + 1];
    char key[MAX_KEY_LEN], value[MAX_VALUE_LEN];
    int ret;

    if (kv_count >= MAX_ENTRIES) {
        return -ENOMEM;  // Memory limit reached
    }

    if (copy_from_user(buffer, user_buffer, len)) {
        return -EFAULT;  // Failed to copy data from user space
    }

    buffer[len] = '\0';  // Null-terminate the string

    // Parse key-value pair from the buffer
    ret = sscanf(buffer, "%31[^,],%31s", key, value);
    if (ret != 2) {
        return -EINVAL;  // Invalid format
    }

    // Store key-value pair in the array
    strncpy(kv_store[kv_count].key, key, MAX_KEY_LEN);
    strncpy(kv_store[kv_count].value, value, MAX_VALUE_LEN);
    kv_count++;

    return len;  // Return the number of bytes written
}

// File operations structure
static struct file_operations fops = {
    .open = kv_open,
    .read = kv_read,
    .write = kv_write,
};

// Module initialization
static int __init kv_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "KV Store device has been registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit kv_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "KV Store device has been unregistered\n");
}

module_init(kv_init);
module_exit(kv_exit);

