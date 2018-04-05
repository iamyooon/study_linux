#include <linux/module.h> // module_init(), module_exit()

static int __init module_example_init(void)
{
	printk("%s\n", __func__);
	return 0;
}

static void __exit module_example_exit(void)
{
	printk("%s\n", __func__);
}

module_init(module_example_init);
module_exit(module_example_exit);
