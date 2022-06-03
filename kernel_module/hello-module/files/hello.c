#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void)
{
    printk("Hello World!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk("Good bye!\n");
}

module_init(hello_init); // when insmod
module_exit(hello_exit); // when rmmod

MODULE_LICENSE("GPL v2");
