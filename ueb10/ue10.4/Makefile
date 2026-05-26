# Kernel-Modul Makefile für zombie_monitor.c
#
# obj-m sagt dem Kernel-Build-System, dass zombie_monitor.o als
# ladbares Modul gebaut werden soll. Daraus entsteht zombie_monitor.ko.

obj-m += zombie_monitor.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
