#include <linux/kernel.h>
#include <linux/module.h>

static int __init mostima_init(void)
{
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

