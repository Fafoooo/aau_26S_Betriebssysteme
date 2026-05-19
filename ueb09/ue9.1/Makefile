# Kernel-Modul Makefile für demo.c
#
# obj-m sagt dem Kernel-Build-System: "Baue diese Datei als ladbares Modul".
# Aus demo.o entsteht dann demo.ko (Kernel Object), das mit insmod
# geladen werden kann.

obj-m += demo.o

# Das eigentliche Bauen passiert über das Kernel-Build-System.
# -C wechselt in das Kernel-Build-Verzeichnis (passend zur laufenden
# Kernel-Version), M=$(PWD) zeigt zurück auf unser Modul-Verzeichnis.
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
