#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define BUFFER_SIZE 100

MODULE_LICENSE("GPL");

static char buffer[BUFFER_SIZE];
static struct kobject *example_kobject;

static ssize_t string_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", buffer);
}

static ssize_t string_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    if (count > BUFFER_SIZE - 1)
        return -EINVAL;

    strncpy(buffer, buf, count);
    buffer[count] = '\0';
    return count;
}

static struct kobj_attribute string_attribute = __ATTR(string, 0660, string_show, string_store);

static int __init sysfs_driver_init(void) {
    example_kobject = kobject_create_and_add("sysfs_example", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    if (sysfs_create_file(example_kobject, &string_attribute.attr)) {
        kobject_put(example_kobject);
        return -ENOMEM;
    }

    return 0;
}

static void __exit sysfs_driver_exit(void) {
    sysfs_remove_file(example_kobject, &string_attribute.attr);
    kobject_put(example_kobject);
}

module_init(sysfs_driver_init);
module_exit(sysfs_driver_exit);

