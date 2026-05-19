# Kernel-Modul Makefile für keylogger.c
#
# Gleiche Struktur wie das Makefile aus 9.1, nur das Ziel heißt
# anders. obj-m sagt dem Kernel-Build-System, dass keylogger.o
# als ladbares Modul gebaut werden soll. Daraus entsteht keylogger.ko,
# das mit insmod geladen werden kann.

obj-m += keylogger.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
