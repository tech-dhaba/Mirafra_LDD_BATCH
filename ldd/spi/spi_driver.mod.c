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
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x646397e6, "_dev_info" },
	{ 0xb94827b9, "devm_kmalloc" },
	{ 0x962310d9, "spi_sync" },
	{ 0x1e898af9, "_dev_err" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x7c5672c3, "driver_unregister" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x4c8e0646, "__spi_register_driver" },
	{ 0xf079b8f9, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("spi:rpi4b_spi_device");

MODULE_INFO(srcversion, "538F975793AC17B2E3207DA");
