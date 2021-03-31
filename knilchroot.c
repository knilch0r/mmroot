#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knilch");
MODULE_DESCRIPTION("a 'make me root' knilchroot");
MODULE_VERSION("0.0");


/* Prototypes for knilchroot functions */
static int knilchroot_open(struct inode *, struct file *);

static int knilchroot_release(struct inode *, struct file *);
static ssize_t knilchroot_read(struct file *, char *, size_t, loff_t *);
static ssize_t knilchroot_write(struct file *, const char *, size_t, loff_t *);

static int major_num;

static struct file_operations file_ops = {
	.read = knilchroot_read,
	.write = knilchroot_write,
	.open = knilchroot_open,
	.release = knilchroot_release
};

static ssize_t knilchroot_read(struct file *f, __user char *b, size_t l, loff_t *o) {
	return 0;
}

static ssize_t knilchroot_write(struct file *f, const char *b, size_t l, loff_t *o) {
	return 0;
}

static int knilchroot_open(struct inode *inode, struct file *file) {
	try_module_get(THIS_MODULE);
	return 0;
}

static int knilchroot_release(struct inode *inode, struct file *file) {
	module_put(THIS_MODULE);
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
	printk(KERN_INFO "knilchroot unloaded");
}

module_init(knilchroot_init);
module_exit(knilchroot_exit);

