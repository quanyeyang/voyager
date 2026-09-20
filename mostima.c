#include "linux/if_ether.h"
#include "linux/printk.h"
#include <linux/kernel.h>
#include <linux/module.h> // basic kernel module API

#include <linux/netdevice.h>
#include <linux/etherdevice.h> // netdevice
#include <linux/string.h>
#include <linux/skbuff.h> // linux socket buffer API

#include <linux/if_ether.h> // Ethernet layer
#include <linux/ip.h> // IP layer

static struct net_device *mostima_dev; // a struct pointer points to our device.

// define actions of the NIC

static int mostima_open(struct net_device *dev)
{
    pr_info("mostima: %s opend!\n", dev->name);
    
    netif_start_queue(dev);
    netif_carrier_on(dev);

    return 0;
}


// skb
//  │
//  ▼
// +-------------------------+
// | Ethernet Header         | 14 bytes
// +-------------------------+
// |                         |
// | ARP / IPv4 / IPv6 ...   |
// |                         |
// +-------------------------+
// skb here is like this.

// 1.and what about the ethernet header?
//   6 bytes          6 bytes          2 bytes
// +--------------+--------------+---------------+
// | Destination  | Source MAC   | EtherType     |
// | MAC          |              |               |
// +--------------+--------------+---------------+

static int mostima_stop(struct net_device *dev)
{
    pr_info("mostima: %s stopped!\n", dev->name);

    netif_carrier_off(dev);
    netif_stop_queue(dev);

    return 0;
}

// when linux wants the device to transmit pkt
static netdev_tx_t mostima_start_xmit(struct sk_buff *skb,
                                struct net_device *dev)
{
    struct ethhdr *ethernet_header;
    u16 protocol;

    pr_info_ratelimited(
        "mostima: %s TX packet: %u bytes\n",
        dev->name,
        skb->len
    );


    // 1.check what the ethernet header would be like.
    ethernet_header = eth_hdr(skb);
    protocol = ntohs(ethernet_header->h_proto);

    pr_info_ratelimited(
        "mostima: TX len=%u, src=%pM, dst=%pM, ethernettype=0x%04x\n", // %pM used to print MAC specificly
        skb->len,
        ethernet_header->h_source,
        ethernet_header->h_dest,
        protocol
    );


    // LWN: if you return TX_OK, you must free skb in this function
    // this means driver receive the packet successfully
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

// operations defined here (function pointers)
static const struct net_device_ops mostima_device_ops = {
    .ndo_open = mostima_open,
    .ndo_stop = mostima_stop,
    .ndo_start_xmit = mostima_start_xmit,
};



static int __init mostima_init(void)
{
    int ret;

    mostima_dev = alloc_etherdev(0);
    if (!mostima_dev) {
        return -ENOMEM;
    }

    // give a name
    strscpy(mostima_dev->name, "mostima%d", IFNAMSIZ);

    // allocate a max address
    eth_hw_addr_random(mostima_dev);

    // assign ops, can't be NULL here
    mostima_dev->netdev_ops = &mostima_device_ops;

    // register the dev to linux networking subsystem
    ret = register_netdev(mostima_dev);
    if (ret) {
        free_netdev(mostima_dev);
        return ret;
    }

    pr_info("mostima inited\n");
    return 0;
}

static void __exit mostima_exit(void)
{
    unregister_netdev(mostima_dev);
    free_netdev(mostima_dev);
    pr_info("mostima:bye!\n");
}

module_init(mostima_init);
module_exit(mostima_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("quanyeyang");
MODULE_DESCRIPTION("Kernel newbee is here...");

