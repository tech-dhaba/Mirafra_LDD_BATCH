# Define the kernel source directory and the name of the module
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m := keyboard_interrupt.o

# The default target to build the module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean target to remove built files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

