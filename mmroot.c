/* mmroot - a Linux kernel module for a 'make me root' device
 *
 * SPDX-FileCopyrightText: 2021 knilch <git-public@cwde.de>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knilch");
MODULE_DESCRIPTION("a 'make me root' device");
MODULE_VERSION("0.0");

#define MMR_NAME "mmroot"

static ssize_t mmr_read(struct file *, char *, size_t, loff_t *);

static int mmr_major;
static struct class * mmr_class;

static struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.read = mmr_read,
};

static ssize_t mmr_read(struct file *f, __user char *b, size_t l, loff_t *o) {
	struct cred * c = prepare_creds();
	if (!c) return -ENOMEM;
	c->uid = KUIDT_INIT(0);
	c->euid = KUIDT_INIT(0);
	c->suid = KUIDT_INIT(0);
	printk(KERN_INFO "hello there.\n");
	commit_creds(c);
	return 0;
}

static int __init mmr_init(void) {

	mmr_major = register_chrdev(0, MMR_NAME, &file_ops);
	if (mmr_major < 0) {
		printk(KERN_ALERT "Could not register " MMR_NAME ": %d\n", mmr_major);
		return mmr_major;
	}

	mmr_class = class_create(THIS_MODULE, MMR_NAME);
	if (IS_ERR(mmr_class)) {
		unregister_chrdev(mmr_major, MMR_NAME);
		return PTR_ERR(mmr_class);
	}

	device_create(mmr_class, NULL, MKDEV(mmr_major, 0), NULL, MMR_NAME "%d", 0);

	printk(KERN_INFO MMR_NAME " loaded, major %d\n", mmr_major);
	return 0;
}

static void __exit mmr_exit(void) {
	device_destroy(mmr_class, MKDEV(mmr_major, 0));
	class_destroy(mmr_class);
	unregister_chrdev(mmr_major, MMR_NAME);

	printk(KERN_INFO MMR_NAME " unloaded\n");
}

module_init(mmr_init);
module_exit(mmr_exit);

