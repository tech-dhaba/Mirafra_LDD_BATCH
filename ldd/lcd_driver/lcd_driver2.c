#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  // for copy_to_user() and copy_from_user()

#define RS  529  // GPIO Pin for RS
#define EN  530 // GPIO Pin for EN
#define D4  534  // GPIO Pin for D4
#define D5  535  // GPIO Pin for D5
#define D6  536  // GPIO Pin for D6
#define D7  537  // GPIO Pin for D7
#define DEVICE_NAME "lcd_driver"  // The name of the device

// Function prototypes
void lcd_send_nibble(int data);
void lcd_send_byte(int data, int mode);
void lcd_initialize(void);
void lcd_clear(void);
void lcd_print(char* str);

static int major;
static char lcd_buffer[16];  // Buffer to store string to be displayed

// File operations
static int lcd_open(struct inode *inode, struct file *file);
static ssize_t lcd_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t lcd_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);
static int lcd_release(struct inode *inode, struct file *file);

// File operations structure
static struct file_operations fops = {
    .open = lcd_open,
    .read = lcd_read,
    .write = lcd_write,
    .release = lcd_release,
};

static int __init lcd_driver_init(void)
{
    int ret;

    // Register the device
    major = register_chrdev(0, DEVICE_NAME, &fops);  // Dynamically allocate major number
    if (major < 0) {
        printk(KERN_ERR "Failed to register the character device\n");
        return major;
    }
    printk(KERN_INFO "LCD Driver registered with major number %d\n", major);

    // Request GPIO pins
    ret = gpio_request(RS, "RS");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for RS\n");
        return ret;
    }
    ret = gpio_request(EN, "EN");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for EN\n");
        gpio_free(RS);
        return ret;
    }
    ret = gpio_request(D4, "D4");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for D4\n");
        gpio_free(RS);
        gpio_free(EN);
        return ret;
    }
    ret = gpio_request(D5, "D5");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for D5\n");
        gpio_free(RS);
        gpio_free(EN);
        gpio_free(D4);
        return ret;
    }
    ret = gpio_request(D6, "D6");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for D6\n");
        gpio_free(RS);
        gpio_free(EN);
        gpio_free(D4);
        gpio_free(D5);
        return ret;
    }
    ret = gpio_request(D7, "D7");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO for D7\n");
        gpio_free(RS);
        gpio_free(EN);
        gpio_free(D4);
        gpio_free(D5);
        gpio_free(D6);
        return ret;
    }

    // Set GPIO pins as output
    gpio_direction_output(RS, 0);
    gpio_direction_output(EN, 0);
    gpio_direction_output(D4, 0);
    gpio_direction_output(D5, 0);
    gpio_direction_output(D6, 0);
    gpio_direction_output(D7, 0);

    // Initialize the LCD
    lcd_initialize();

    return 0;
}

static void __exit lcd_driver_exit(void)
{
    printk(KERN_INFO "LCD Driver Exited\n");

    // Free the GPIO pins
    gpio_free(RS);
    gpio_free(EN);
    gpio_free(D4);
    gpio_free(D5);
    gpio_free(D6);
    gpio_free(D7);

    // Unregister the device
    unregister_chrdev(major, DEVICE_NAME);
}

// Open function
static int lcd_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LCD Device Opened\n");
    return 0;
}

// Read function (Returns a simple string for testing)
static ssize_t lcd_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    // Copy the string from the buffer to user space
    if (copy_to_user(buf, lcd_buffer, strlen(lcd_buffer))) {
        return -EFAULT;
    }
    return strlen(lcd_buffer);
}

// Write function (Accepts string to display on the LCD)
static ssize_t lcd_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    // Copy the string from user space to buffer
    if (copy_from_user(lcd_buffer, buf, len)) {
        return -EFAULT;
    }
    lcd_buffer[len] = '\0';  // Null-terminate the string
    lcd_print(lcd_buffer);   // Send the string to the LCD
    return len;
}

// Release function
static int lcd_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LCD Device Closed\n");
    return 0;
}

// Send a 4-bit nibble to the LCD
void lcd_send_nibble(int data)
{
    gpio_set_value(D4, (data >> 0) & 0x01);
    gpio_set_value(D5, (data >> 1) & 0x01);
    gpio_set_value(D6, (data >> 2) & 0x01);
    gpio_set_value(D7, (data >> 3) & 0x01);
}

// Send a byte (8-bit data/command) to the LCD
void lcd_send_byte(int data, int mode)
{
    gpio_set_value(RS, mode);  // Set RS for data (1) or command (0)
    lcd_send_nibble(data >> 4);  // Send the high nibble
    gpio_set_value(EN, 1);  // Enable pulse
    udelay(10);
    gpio_set_value(EN, 0);  // Disable pulse
    lcd_send_nibble(data);  // Send the low nibble
    gpio_set_value(EN, 1);  // Enable pulse
    udelay(10);
    gpio_set_value(EN, 0);  // Disable pulse
}

// Initialize the LCD in 4-bit mode
void lcd_initialize(void)
{
    mdelay(50);  // Wait for the LCD to power up

    lcd_send_nibble(0x03);  // Initialize the LCD in 8-bit mode
    mdelay(5);
    lcd_send_nibble(0x03);  // Initialize again
    mdelay(5);
    lcd_send_nibble(0x03);  // Third time
    mdelay(5);
    lcd_send_nibble(0x02);  // Switch to 4-bit mode

    lcd_send_byte(0x28, 0);  // 4-bit, 2 lines, 5x7 font
    lcd_send_byte(0x0C, 0);  // Display on, cursor off
    lcd_send_byte(0x06, 0);  // Increment cursor, no shift
    lcd_send_byte(0x01, 0);  // Clear the display
    mdelay(2);
}

// Print a string on the LCD
void lcd_print(char* str)
{
    while (*str) {
        lcd_send_byte(*str, 1);  // Send each character in data mode (RS=1)
        str++;
    }
}

module_init(lcd_driver_init);
module_exit(lcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Raspberry Pi GPIO LCD Driver with File Operations");
