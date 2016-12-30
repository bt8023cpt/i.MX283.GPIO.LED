obj-m := led.o

KDIR := /home/uesr/workspace/i.MX283/linux-2.6.35.3
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
