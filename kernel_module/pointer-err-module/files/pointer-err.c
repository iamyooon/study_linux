#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/slab.h>


static int __init ptr_init(void)
{
	char* buff;

	printk("kmalloc test\n");

        buff = kmalloc(32 * PAGE_SIZE, GFP_KERNEL);

	if(IS_ERR(buff)) {
		pr_err("Error returned is %d!", PTR_ERR(buff));
		return PTR_ERR(buff);
	}

	if(buff != NULL)
	{
		sprintf(buff, "kmalloc test ok \n");
		printk(buff);
	}

	kfree(buff);

	return 0;
}

static void __exit ptr_exit(void)
{
	pr_err("Good bye!");
}

module_init(ptr_init); // when insmod
module_exit(ptr_exit); // when rmmod

MODULE_LICENSE("GPL v2");
