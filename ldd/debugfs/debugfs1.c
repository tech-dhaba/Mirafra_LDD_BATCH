#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

#define MODULE_NAME "debuggfs"
#define PARENT_DIR "parent_dir"

static struct dentry *dir, *parent_dir, *file;
static int debugfs_value = 0;  /* Declare the variable */

/* Read function */
static ssize_t debugfs_read(struct file *fp, char __user *user_buffer, size_t count, loff_t *position) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "Current value: %d\n", debugfs_value);

    return simple_read_from_buffer(user_buffer, count, position, buffer, len);
}

/* Write function */
static ssize_t debugfs_write(struct file *fp, const char __user *user_buffer, size_t count, loff_t *position) {
    char buffer[64];

    if (count >= sizeof(buffer))
        return -EINVAL;

    if (copy_from_user(buffer, user_buffer, count))
        return -EFAULT;

    buffer[count] = '\0';
    if (kstrtoint(buffer, 10, &debugfs_value))  /* Update the variable */
        return -EINVAL;

    return count;
}

/* File operations structure */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = debugfs_read,
    .write = debugfs_write,  /* Add write operation here */
};

/* Module initialization */
static int __init debugfs_example_init(void) {
    /* First, try to lookup the parent directory */
    parent_dir = debugfs_lookup(PARENT_DIR, NULL);  // NULL means lookup at the top level of debugfs

    if (!parent_dir) {
        /* If parent directory doesn't exist, create it */
        parent_dir = debugfs_create_dir(PARENT_DIR, NULL);  // Create at the top level of debugfs
        if (!parent_dir) {
            pr_err("Failed to create parent debugfs directory\n");
            return -ENOMEM;
        }
    }

    /* Now, create a subdirectory under the parent directory */
    dir = debugfs_create_dir(MODULE_NAME, parent_dir);
    if (!dir) {
        pr_err("Failed to create debugfs directory\n");
        return -ENOMEM;
    }

    /* Create a file inside the newly created directory */
    file = debugfs_create_file("value", 0644, dir, NULL, &fops);
    if (!file) {
        pr_err("Failed to create debugfs file\n");
        debugfs_remove_recursive(dir);
        return -ENOMEM;
    }

    pr_info("debugfs_example module loaded\n");
    return 0;
}

/* Module exit */
static void __exit debugfs_example_exit(void) {
    debugfs_remove_recursive(dir);  /* Remove all debugfs entries  */
    pr_info("debugfs_example module unloaded\n");
}

module_init(debugfs_example_init);
module_exit(debugfs_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Kumar Reddy");
MODULE_DESCRIPTION("A simple debugfs example with specific dentry");

