#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Barynina Alina <alina00427@gmail.com>");
MODULE_DESCRIPTION("print_hello() function");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(event_list);

void print_hello(unsigned int msg_count)
{
	int i;

	for (i = 0; i < msg_count; i++) {
		struct event_data *event = kmalloc(sizeof(*event), GFP_KERNEL);

		event->timestamp = ktime_get();
		list_add(&event->list, &event_list);

		pr_info("Hello, world!\n");

		ktime_t print_end = ktime_get();

		event->print_duration = ktime_sub(print_end, event->timestamp);
	}
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");

	return 0;
}

static void __exit hello1_exit(void)
{
	struct event_data *event, *temp;

	list_for_each_entry_safe(event, temp, &event_list, list) {
		ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

		pr_info(
			"Event time: %lld ns, Print time: %lld ns\n",
			ktime_to_ns(duration),
			ktime_to_ns(event->print_duration)
		);

		list_del(&event->list);
		kfree(event);
	}

	pr_info("Unloading hello2 module\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

MODULE_ALIAS("hello1");
