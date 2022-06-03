#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>

#define ww_ptr(fmt, ...) \
       printk(KERN_ERR "[%s:%d] wewake " pr_fmt(fmt),__func__, __LINE__ ##__VA_ARGS__)

static int __init hello_init(void)
{
	ww_ptr("Hello World!");
	return 0;
}

static void __exit hello_exit(void)
{
	ww_ptr("Good bye!");
}

module_init(hello_init); // when insmod
module_exit(hello_exit); // when rmmod

MODULE_LICENSE("GPL v2");
