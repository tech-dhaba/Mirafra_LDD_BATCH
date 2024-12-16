#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include<linux/kernel.h>
#include<linux/fs.h>

#define PROC_FILENAME "my_proc_file"
#define PROC_MAX_SIZE 1024

static char proc_data[PROC_MAX_SIZE];
static struct proc_dir_entry *proc_file;

/* Read function */
static ssize_t proc_read(struct file *file, char __user *user_buffer, size_t count, loff_t *position) {
    pr_info("read function %s",user_buffer);
    return simple_read_from_buffer(user_buffer, count, position, proc_data, strlen(proc_data));
}

/* Write function */
static ssize_t proc_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *position) {
    if (count >= PROC_MAX_SIZE)
        return -EINVAL;

    if (copy_from_user(proc_data, user_buffer, count))
        return -EFAULT;

    proc_data[count] = '\0'; // Null-terminate the string
    pr_info("write function %s",proc_data);
    return count;
}

/* File operations structure */
/*static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};*/
static const struct file_operations proc_fops = {
    .read = proc_read,
    .write = proc_write,
};


/* Module initialization */
static int __init procfs_example_init(void) {
    proc_file = proc_create(PROC_FILENAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    strcpy(proc_data, "Hello from procfs!\n");
    pr_info("proc_data %s\n",proc_data);
    pr_info("procfs_example module loaded. File: /proc/ %s\n",PROC_FILENAME);
    return 0;
}

/* Module cleanup */
static void __exit procfs_example_exit(void) {
    proc_remove(proc_file);
    pr_info("procfs_example module unloaded\n");
}

module_init(procfs_example_init);
module_exit(procfs_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gopikrishna");
MODULE_DESCRIPTION("A simple procfs example module");
