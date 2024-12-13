#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xdd8f8694, "module_layout" },
	{ 0x87d7b3f5, "kthread_stop" },
	{ 0x22e92418, "device_destroy" },
	{ 0x6bb70076, "wake_up_process" },
	{ 0xa6521794, "kthread_create_on_node" },
	{ 0x977f511b, "__mutex_init" },
	{ 0xb65e5a32, "class_destroy" },
	{ 0x7749276a, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x2871e975, "__class_create" },
	{ 0x9a19ed29, "__register_chrdev" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xcf2a6966, "up" },
	{ 0x5ab904eb, "pv_ops" },
	{ 0xdbf17652, "_raw_spin_lock" },
	{ 0x81b395b3, "down_interruptible" },
	{ 0xf9a482f9, "msleep" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0xc5850110, "printk" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "697002402B0FAB555E40405");
