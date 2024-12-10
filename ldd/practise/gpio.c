#include<linux/gpio.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>

#define GPIO 17

static int gpio_open(struct inode *inode, struct file *file) {
    pr_info("GPIO Device opened\n");
    return 0;
}

static int gpio_close(struct inode *inode, struct file *file) {
    pr_info("GPIO Device closed\n");
    return 0;
}

static ssize_t gpio_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
int value=gpio_get_value(GPIO);

if(copy_to_user(buf,&value,2))
{
printk(KERN_ERR"data is not copied into user buffer\n");
gpio_free(GPIO);
return -EFAULT;
}

printk("GPIO Value is %d\n",value);
return 2;
}

static ssize_t gpio_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
char buffer[10]={0};
if(copy_from_user(buffer,buf,len))
{
printk(KERN_ERR"data is not copied into kernel buffer\n");
gpio_free(GPIO);
return -EFAULT;
}
buffer[len]='\0';
if(buffer[0]=='0')
gpio_set_value(GPIO,0);
else if(buffer[1]=='1')
 gpio_set_value(GPIO,1);
else
{
   printk("chose 0 or 1 \n");
   return -EINVAL;
}
printk("GPIO SET value is %c\n",buffer[0]);
return 2;
}

static int gpio(void)
{
  if(!gpio_is_valid(GPIO))
{
  printk("gpio is not valid\n");
  return -EFAULT;
}
printk("GPIO is valid\n");

if(gpio_request(GPIO,"GPIO_DRIVER")<0)
{
printk("GPIO pin is busy/fail(request)\n");
return -EFAULT;
}
printk("GPIO pin is request successfully\n");

return 1;
}

static int gpio_init(void)
{
  int value=gpio();
   if(value!=1)
    return -EFAULT;
  value=gpio_direction_output(GPIO,1);
  if(value!=0)
  return -EFAULT;
printk("GPIO loaded successfully\n");
  return 0;
}

static void gpio_exit(void)
{
   gpio_free(GPIO);
   printk("GPIO unloaded\n");
}

module_init(gpio_init);
module_exit(gpio_exit);
