#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Barynina Alina <alina00427@gmail.com>");
MODULE_DESCRIPTION("Hello, my module!");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int msg_count = 1;
module_param(msg_count, uint, 0444);
MODULE_PARM_DESC(msg_count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
	pr_info("hello1 module loaded\n");

	if (msg_count == 0 || (msg_count >= 5 && msg_count <= 10)) {
		pr_warn("Warning: count of messages is 0 or between 5 and 10, continuing...\n");
	} else if (msg_count > 10) {
		pr_err("Error: count of messages is greater than 10, module cannot be loaded\n");
		return -EINVAL;
	}

	print_hello(msg_count);

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("Unloading hello2 module\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_ALIAS("hello2");
