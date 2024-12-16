#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h> // for Ethernet headers

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
    int packet_size = 64; // Example packet size (can vary based on your data)
    
    pr_info("Transmitting packet\n");

    // Allocate the skb (this is where the actual data will be stored)
    skb = dev_alloc_skb(packet_size + ETH_HLEN);  // + ETH_HLEN for Ethernet header
    if (!skb) {
        pr_err("Failed to allocate skb\n");
        return NETDEV_TX_BUSY;  // Indicate failure to transmit
    }

    // Fill in the Ethernet header
    eth_header = (struct ethhdr *)skb_push(skb, ETH_HLEN);  // Push Ethernet header onto skb
    memcpy(eth_header->h_dest, "\x01\x02\x03\x04\x05\x06", ETH_ALEN);  // Destination MAC address
    memcpy(eth_header->h_source, dev->dev_addr, ETH_ALEN);  // Source MAC address (device's MAC)
    eth_header->h_proto = htons(ETH_P_IP);  // Set EtherType to IP

    // Fill the skb with the payload data (e.g., IP packet, here we'll just use dummy data)
    data = skb->data + ETH_HLEN;  // Skip Ethernet header to fill the data section
    memset(data, 0xFF, packet_size);  // Example: Fill the data with dummy bytes

    // Set the length of the packet
    skb_put(skb, packet_size);

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

    // Allocate and register net_device
    dev = alloc_netdev(0, "mynet%d", NET_NAME_UNKNOWN, mynet_setup);
    if (register_netdev(dev)) {
        pr_info("mynet: Failed to register device\n");
        free_netdev(dev);
        return -ENODEV;
    }
    pr_info("Module loaded, device registered: %p\n", dev);
    return 0;
}

// Module exit
static void __exit mynet_exit(void) {
    struct net_device *dev = first_net_device(&init_net);
    unregister_netdev(dev); // Unregister the device
    free_netdev(dev); // Free memory
    pr_info("Module unloaded\n");
}

module_init(mynet_init);
module_exit(mynet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Custom Network Driver with Sender and Receiver");

