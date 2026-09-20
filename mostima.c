#include "linux/printk.h"
#include <linux/kernel.h>
#include <linux/module.h> // basic kernel module API

#include <linux/netdevice.h>
#include <linux/etherdevice.h> // netdevice
#include <linux/string.h>
#include <linux/skbuff.h> // linux socket buffer API

static struct net_device *mostima_dev; // a struct pointer points to our device.

// define actions of the NIC

static int mostima_open(struct net_device *dev)
{
    pr_info("mostima: %s opend!\n", dev->name);
    
    netif_start_queue(dev);
    netif_carrier_on(dev);

    return 0;
}

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
    pr_info_ratelimited(
        "mostima: %s TX packet: %u bytes\n",
        dev->name,
        skb->len
    );

    dev_kfree_skb(skb);

    // LWN: if you return TX_OK, you must free skb in this function
    // this means driver receive the packet successfully
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

