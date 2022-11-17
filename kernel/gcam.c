#include <linux/module.h> // module_init(), module_exit()
#include <asm/uaccess.h>
#include <linux/fs.h>

static int virtio_file_write(char *path, uint8_t *buf, uint32_t size)
{
	struct file *fp;
	int ret = 0;

	fp = filp_open(path, O_RDWR | O_CREAT, 0644);
	if (IS_ERR(fp)) {
		ret = PTR_ERR(fp);
	}
	else {
		loff_t pos = 0;
		ret = kernel_write(fp, buf, size, &pos);
		if (ret < 0)
			printk("Error writing plane data\n");

		filp_close(fp, NULL);
	}
	return ret;
}

static int __init gcam_init(void)
{
	char bufs[100];

	printk("%s\n", __func__);
	strcpy(bufs, "hello my world");
	virtio_file_write("/media/gcam_test", bufs, strlen(bufs));
	return 0;
}

static void __exit gcam_exit(void)
{
	printk("%s\n", __func__);
}

module_init(gcam_init);
module_exit(gcam_exit);
