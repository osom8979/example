## Device Driver build makefile.

KDIR  := $(KERN_DIR)
obj-m := m01.o

all:
	@echo build.
	make -C $(KDIR) M=$(PWD) modules

clean:
	@echo clean.
	@rm -rf *.o
	@rm -rf *.ko
	@rm -rf *.mod.*
	@rm -rf .*.cmd
	@rm -rf *.order
	@rm -rf *.symvers
	@rm -rf .tmp_versions/
