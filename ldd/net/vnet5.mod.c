#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0x275decd1, "free_netdev" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xfd6ef4ae, "ether_setup" },
	{ 0xfa21a6ea, "init_net" },
	{ 0xa730d484, "sysfs_remove_group" },
	{ 0x37bba26d, "__netdev_alloc_skb" },
	{ 0x54300b50, "skb_push" },
	{ 0x69acdf38, "memcpy" },
	{ 0x2f8c6f39, "skb_put" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xcb9afc93, "consume_skb" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x2c26c5d, "alloc_netdev_mqs" },
	{ 0xfd06e1e7, "register_netdev" },
	{ 0x1af94c3a, "sysfs_create_group" },
	{ 0x628d88d6, "unregister_netdev" },
	{ 0xf079b8f9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "95421ED5C6FA36BE09397AE");
