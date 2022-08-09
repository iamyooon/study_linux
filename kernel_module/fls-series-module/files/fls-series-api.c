#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>
//#include <string.h>

#define ww_ptr(fmt, ...) \
       printk(KERN_ERR "[%s:%d] wewake " pr_fmt(fmt),__func__, __LINE__ ##__VA_ARGS__)

static int __init fls_series_init(void)
{
	ww_ptr("Hello fls-series-module \n");
	printk("ffs() : find first bit set\n");
	printk("0x8010,0b1000000000010000\n");
	printk("ffs(0x8010) = %d\n", ffs(0x8010));
	printk("================================================\n");
	printk("fls() : find last (most-significant) bit set\n");
	printk("0x1000, 0b1000000000000");
	printk("fls(0x1000) = %d\n", fls(0x1000));
	printk("================================================\n");
	printk(" fls64 - find last set bit in a 64-bit word\n");
	printk("0x1111120000000, 0b1000100010001000100100000000000000000000000000000\n");
	printk("fls64(0x1111120000000) = %d\n", fls64(0x1111120000000));

	return 0;
}

static void __exit fls_series_exit(void)
{
	ww_ptr("Good bye!");
}

module_init(fls_series_init); // when insmod
module_exit(fls_series_exit); // when rmmod

MODULE_LICENSE("GPL v2");
