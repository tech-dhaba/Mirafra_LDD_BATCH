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



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\xe6\x97\x63\x64"
	"_dev_info\0\0\0"
	"\x1c\x00\x00\x00\x68\x3f\x63\xca"
	"simple_attr_open\0\0\0\0"
	"\x18\x00\x00\x00\xb9\x27\x48\xb9"
	"devm_kmalloc\0\0\0\0"
	"\x1c\x00\x00\x00\x95\x45\xdc\x4e"
	"debugfs_create_dir\0\0"
	"\x1c\x00\x00\x00\x28\xff\xe2\x1b"
	"debugfs_create_file\0"
	"\x14\x00\x00\x00\xd9\x10\x23\x96"
	"spi_sync\0\0\0\0"
	"\x14\x00\x00\x00\xf9\x8a\x89\x1e"
	"_dev_err\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x1c\x00\x00\x00\xc3\x72\x56\x7c"
	"driver_unregister\0\0\0"
	"\x1c\x00\x00\x00\xab\xbf\x77\xb3"
	"generic_file_llseek\0"
	"\x1c\x00\x00\x00\xad\x81\xd0\xc5"
	"simple_attr_read\0\0\0\0"
	"\x1c\x00\x00\x00\x90\xb1\x1b\x53"
	"simple_attr_write\0\0\0"
	"\x1c\x00\x00\x00\x6a\x2e\x27\xc2"
	"simple_attr_release\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x20\x00\x00\x00\x46\x06\x8e\x4c"
	"__spi_register_driver\0\0\0"
	"\x18\x00\x00\x00\x10\xd4\x5d\xad"
	"debugfs_remove\0\0"
	"\x18\x00\x00\x00\x3a\x0a\xd8\xfc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("spi:rpi4b_spi_device");

MODULE_INFO(srcversion, "C7056CB2E5875B5C1A222E6");
