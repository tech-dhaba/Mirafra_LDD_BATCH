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
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x9a19ed29, "__register_chrdev" },
	{ 0xdbb1fddb, "gpiod_direction_output_raw" },
	{ 0xfe990052, "gpio_free" },
	{ 0xd4d7e65a, "gpiod_direction_input" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x1e4e6470, "gpiod_get_raw_value" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc9103412, "gpiod_set_raw_value" },
	{ 0x269e00d4, "gpio_to_desc" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DC9C7F4B8D946FF7E4E635D");
