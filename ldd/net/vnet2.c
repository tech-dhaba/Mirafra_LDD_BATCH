#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define DEVICE_NAME "pingdriver"
#define PING_BUFFER_SIZE 1024 // Adjust the buffer size as required

// Forward declaration for the write_to_buffer function
ssize_t write_to_buffer(const char __user *data, size_t len);

// Custom net_device_ops function declarations
static int mynet_open(struct net_device *dev);
static int mynet_stop(struct net_device *dev);
static netdev_tx_t mynet_start_xmit(struct sk_buff *skb, struct net_device *dev);

// Define net_device_ops structure
static const struct net_device_ops mynet_netdev_ops = {
    .ndo_open = mynet_open,
    .ndo_stop = mynet_stop,
    .ndo_start_xmit = mynet_start_xmit,
};

// Shared buffer to hold the data
static char ping_buffer[PING_BUFFER_SIZE];
static int buffer_len = 0;  // Length of data in the buffer
static struct mutex buffer_mutex;  // Mutex to protect access to the buffer

// Sysfs attributes
static ssize_t pingdriver_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t pingdriver_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

// Define sysfs attributes
static struct kobj_attribute pingdriver_attr = __ATTR(pingdriver, 0664, pingdriver_show, pingdriver_store);

// Define the sysfs attribute group
static struct attribute *pingdriver_attrs[] = {
    &pingdriver_attr.attr,
    NULL,
};

static struct attribute_group pingdriver_attr_group = {
    .name = "pingdriver",
    .attrs = pingdriver_attrs,
};

// Open function
static int mynet_open(struct net_device *dev) {
    netif_start_queue(dev); // Start the transmit queue
    pr_info("Device opened, you can send data\n");
    return 0;
}

// Stop function
static int mynet_stop(struct net_device *dev) {
    netif_stop_queue(dev); // Stop the transmit queue
    pr_info("Device closed\n");
    return 0;
}

// Transmit function
static netdev_tx_t mynet_start_xmit(struct sk_buff *skb, struct net_device *dev) {
    struct ethhdr *eth_header; // Ethernet header structure
    unsigned char *data; // Pointer to packet data
    
    pr_info("Transmitting packet from buffer\n");

    if (buffer_len == 0) {
        pr_err("No data to transmit\n");
        dev_kfree_skb(skb);
        return NETDEV_TX_BUSY;
    }

    // Allocate the skb (this is where the actual data will be stored)
    skb = dev_alloc_skb(buffer_len + ETH_HLEN);  // + ETH_HLEN for Ethernet header
    if (!skb) {
        pr_err("Failed to allocate skb\n");
        return NETDEV_TX_BUSY;  // Indicate failure to transmit
    }

    // Fill in the Ethernet header
    eth_header = (struct ethhdr *)skb_push(skb, ETH_HLEN);  // Push Ethernet header onto skb
    memcpy(eth_header->h_dest, "\x01\x02\x03\x04\x05\x06", ETH_ALEN);  // Destination MAC address
    memcpy(eth_header->h_source, dev->dev_addr, ETH_ALEN);  // Source MAC address (device's MAC)
    eth_header->h_proto = htons(ETH_P_IP);  // Set EtherType to IP

    // Fill the skb with the buffered data
    data = skb->data + ETH_HLEN;  // Skip Ethernet header to fill the data section
    memcpy(data, ping_buffer, buffer_len);  // Copy data from buffer

    // Set the length of the packet
    skb_put(skb, buffer_len);

    // Submit the packet to the network stack (this actually transmits it)
    netif_trans_update(dev);  // Update the transmit queue length
    dev->stats.tx_bytes += skb->len;  // Update byte count
    dev->stats.tx_packets++;  // Update packet count

    // Pass the skb to the network stack for transmission
    if (dev->netdev_ops->ndo_start_xmit) {
        dev->netdev_ops->ndo_start_xmit(skb, dev);
    }

    // Free the skb after transmission
    dev_kfree_skb(skb);

    return NETDEV_TX_OK;  // Indicate successful transmission
}

// Setup function
static void mynet_setup(struct net_device *dev) {
    ether_setup(dev); // Initialize Ethernet settings
    dev->netdev_ops = &mynet_netdev_ops; // Assign net_device_ops
    dev->flags |= IFF_NOARP; // Disable ARP
    dev->features |= NETIF_F_HW_CSUM; // Enable checksumming
}

// Module init
static int __init mynet_init(void) {
    struct net_device *dev;

    // Initialize the buffer mutex
    mutex_init(&buffer_mutex);

    // Allocate and register net_device
    dev = alloc_netdev(0, "mynet%d", NET_NAME_UNKNOWN, mynet_setup);
    if (register_netdev(dev)) {
        pr_info("mynet: Failed to register device\n");
        free_netdev(dev);
        return -ENODEV;
    }
    pr_info("Module loaded, device registered: %p\n", dev);

    // Create sysfs entries under /sys/class/net/mynet0
    int ret=sysfs_create_group(&dev->dev.kobj, &pingdriver_attr_group);
    if(ret==-1)
    {
	    pr_info("faield to create the group \n");
    }
    return 0;
}

// Module exit
static void __exit mynet_exit(void) {
    struct net_device *dev = first_net_device(&init_net);
    unregister_netdev(dev); // Unregister the device
    free_netdev(dev); // Free memory
    pr_info("Module unloaded\n");

    // Remove sysfs entries
    sysfs_remove_group(&dev->dev.kobj, &pingdriver_attr_group);
}

// Function to write data to the buffer
ssize_t write_to_buffer(const char __user *data, size_t len) {
    if (len > PING_BUFFER_SIZE)
        return -EINVAL;  // Prevent buffer overflow

    // Lock the buffer before writing
    mutex_lock(&buffer_mutex);

    // Copy the data from user space to kernel space
    if (copy_from_user(ping_buffer, data, len)) {
        mutex_unlock(&buffer_mutex);
        return -EFAULT;
    }

    buffer_len = len;  // Set the buffer length

    // Unlock the buffer after writing
    mutex_unlock(&buffer_mutex);

    pr_info("Buffer updated with data: %s\n", ping_buffer);
    return len;
}

// Sysfs show function
static ssize_t pingdriver_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return snprintf(buf, PING_BUFFER_SIZE, "%s\n", ping_buffer);
}

// Sysfs store function
static ssize_t pingdriver_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    return write_to_buffer(buf, count);
}

module_init(mynet_init);
module_exit(mynet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Custom Network Driver with Buffer to Send Data");

