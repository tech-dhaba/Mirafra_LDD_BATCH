#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include<linux/delay.h>
#include <linux/slab.h>
#include <linux/kobject.h>

#define RS_PIN 21
#define EN_PIN 20
#define D4_PIN 26
#define D5_PIN 19
#define D6_PIN 13
#define D7_PIN 6

static struct kobject *lcd_kobj;

/* Utility to set GPIO values   */
static void lcd_gpio_write(int pin, int value) {
    gpio_set_value(pin, value);
}

/* Pulse the Enable pin   */
static void lcd_enable_pulse(void) {
    lcd_gpio_write(EN_PIN, 1);
    udelay(1);
    lcd_gpio_write(EN_PIN, 0);
    udelay(50);
}

/* Send 4 bits to the LCD   */
static void lcd_send_nibble(uint8_t nibble) {
    lcd_gpio_write(D4_PIN, (nibble >> 0) & 1);
    lcd_gpio_write(D5_PIN, (nibble >> 1) & 1);
    lcd_gpio_write(D6_PIN, (nibble >> 2) & 1);
    lcd_gpio_write(D7_PIN, (nibble >> 3) & 1);
    lcd_enable_pulse();
}

/* Send a byte (command/data) to the LCD  */
static void lcd_send_byte(uint8_t byte, int is_data) {
    lcd_gpio_write(RS_PIN, is_data);
    lcd_send_nibble(byte >> 4);  /* High nibble */
    lcd_send_nibble(byte & 0x0F); /* Low nibble  */
    udelay(2000); /* Delay for command execution  */
}

/* Sysfs write functions  */
static ssize_t command_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    unsigned long cmd;
    if (kstrtoul(buf, 10, &cmd)) {
        return -EINVAL;
    }
    lcd_send_byte(cmd, 0); /*   Send command    */
    return count;
}

static ssize_t data_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    if (count == 0) return 0;
    lcd_send_byte(buf[0], 1); /* Send first byte as data   */
    return count;
}

/* Sysfs attributes   */
static struct kobj_attribute command_attr = __ATTR_WO(command);
static struct kobj_attribute data_attr = __ATTR_WO(data);

/* Initialize GPIO pins   */
static int lcd_gpio_init(void) {
    int ret;

    int pins[] = {RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN};
    for (int i = 0; i < ARRAY_SIZE(pins); i++) {
        ret = gpio_request(pins[i], "lcd");
        if (ret) {
            pr_err("Failed to request GPIO %d\n", pins[i]);
            return ret;
        }
        gpio_direction_output(pins[i], 0);
    }

    return 0;
}

/* Release GPIO pins  */
static void lcd_gpio_free(void) {
    int pins[] = {RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN};
    for (int i = 0; i < ARRAY_SIZE(pins); i++) {
        gpio_free(pins[i]);
    }
}

/*  Initialize the LCD   */
static void lcd_initialize(void) {
    msleep(15);  /*  Wait for LCD power-up  */
    lcd_send_nibble(0x03);
    msleep(5);
    lcd_send_nibble(0x03);
    udelay(100);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02); 
    /* Set 4-bit mode  */
    lcd_send_byte(0x28, 0); 
    /* Function set: 2-line, 4-bit, 5x8 dots */
    lcd_send_byte(0x0C, 0); 
    /* Display ON, cursor OFF  */
    lcd_send_byte(0x06, 0); 
    /* Entry mode: increment, no shift   */
    lcd_send_byte(0x01, 0); /* Clear display  */
}

/* Module initialization */
static int __init lcd_driver_init(void) {
    int ret;

    ret = lcd_gpio_init();
    if (ret) return ret;

    lcd_initialize();

    /*  Create sysfs entries   */
    lcd_kobj = kobject_create_and_add("lcd", kernel_kobj);
    if (!lcd_kobj) {
        pr_err("Failed to create kobject\n");
        lcd_gpio_free();
        return -ENOMEM;
    }

    ret = sysfs_create_file(lcd_kobj, &command_attr.attr);
    if (ret) goto err_remove_kobj;

    ret = sysfs_create_file(lcd_kobj, &data_attr.attr);
    if (ret) goto err_remove_kobj;

    pr_info("LCD driver loaded\n");
    return 0;

err_remove_kobj:
    kobject_put(lcd_kobj);
    lcd_gpio_free();
    return ret;
}

/* Module cleanup */
static void __exit lcd_driver_exit(void) {
    sysfs_remove_file(lcd_kobj, &command_attr.attr);
    sysfs_remove_file(lcd_kobj, &data_attr.attr);
    kobject_put(lcd_kobj);
    lcd_gpio_free();
    pr_info("LCD driver unloaded\n");
}

module_init(lcd_driver_init);
module_exit(lcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Sysfs-based LCD Driver for Raspberry Pi");
