obj-m += knilchroot.o

KBPATH := /lib/modules/$(shell uname -r)/build

ifeq ($(wildcard $(KBPATH)),)
$(error $(KBPATH) not found)
endif

all:
	make -C $(KBPATH) M=$(PWD) modules

clean:
	make -C $(KBPATH) M=$(PWD) clean
