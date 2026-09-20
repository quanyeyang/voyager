#include <linux/kernel.h>
#include <linux/module.h> // basic kernel module API

#include <linux/netdevice.h>
#include <linux/etherdevice.h> // netdevice

static struct net_device *mostima_dev; // a struct pointer points to our device.



static int __init mostima_init(void)
{
    int ret;

    mostima_dev = alloc_etherdev(0);
    

    pr_info("mostima inited\n");
    return 0;
}

static void __exit mostima_exit(void)
{
    pr_info("mostima:bye!\n");
}

module_init(mostima_init);
module_exit(mostima_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("quanyeyang");
MODULE_DESCRIPTION("Kernel newbee is here...");

