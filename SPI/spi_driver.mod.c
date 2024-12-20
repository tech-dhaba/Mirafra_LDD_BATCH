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
	"\x14\x00\x00\x00\x46\x90\x16\x6d"
	"_dev_info\0\0\0"
	"\x1c\x00\x00\x00\x38\x92\xeb\xaf"
	"simple_attr_open\0\0\0\0"
	"\x18\x00\x00\x00\x5e\xd0\xc5\xfe"
	"devm_kmalloc\0\0\0\0"
	"\x1c\x00\x00\x00\x37\xcf\xee\x07"
	"debugfs_create_dir\0\0"
	"\x1c\x00\x00\x00\x1d\x59\x64\xf0"
	"debugfs_create_file\0"
	"\x14\x00\x00\x00\x67\xf4\x6f\x15"
	"spi_sync\0\0\0\0"
	"\x14\x00\x00\x00\x10\xf5\xd0\x5b"
	"_dev_err\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x1c\x00\x00\x00\xdc\x1b\x48\xc1"
	"driver_unregister\0\0\0"
	"\x1c\x00\x00\x00\x1f\x03\x32\x9d"
	"generic_file_llseek\0"
	"\x1c\x00\x00\x00\xeb\xc3\xa0\x5d"
	"simple_attr_read\0\0\0\0"
	"\x1c\x00\x00\x00\x6d\x02\xf6\xf0"
	"simple_attr_write\0\0\0"
	"\x1c\x00\x00\x00\x9c\xb7\x04\xeb"
	"simple_attr_release\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x20\x00\x00\x00\xd9\x0b\x21\x44"
	"__spi_register_driver\0\0\0"
	"\x18\x00\x00\x00\xeb\x88\x0c\xfb"
	"debugfs_remove\0\0"
	"\x18\x00\x00\x00\xd7\xd3\x75\x6d"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("spi:rpi4b_spi_device");

MODULE_INFO(srcversion, "D3ADBC51FE9E047821AAF59");
