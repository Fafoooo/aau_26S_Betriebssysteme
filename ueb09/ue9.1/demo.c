// Ü 9.1 - Erstes Kernel-Modul
//
//
// === Aufgabe ===
//
// Ein einfaches "Hello World"-Kernel-Modul. Beim Laden wird
// "demo module loaded" ins Kernel-Log geschrieben, beim Entladen
// "demo module unloaded".
//
//
// === Voraussetzungen ===
//
// Kernel-Build-Tools installieren (einmalig):
//
//   sudo apt-get install build-essential linux-headers-$(uname -r)
//
//
// === Bauen, laden, beobachten, entladen ===
//
//   make                         # baut demo.ko aus demo.c
//   sudo insmod demo.ko          # Modul laden
//   dmesg | tail                 # "demo module loaded" anzeigen
//   sudo rmmod demo              # Modul entladen
//   dmesg | tail                 # "demo module unloaded" anzeigen
//   make clean                   # Build-Artefakte aufräumen
//
//
// === Aufbau eines Kernel-Moduls ===
//
//   - module_init(fn):  Funktion fn wird beim Laden aufgerufen
//   - module_exit(fn):  Funktion fn wird beim Entladen aufgerufen
//   - printk(...):      Kernel-Variante von printf, schreibt ins Kernel-Log
//   - MODULE_LICENSE:   Lizenzangabe (ohne GPL gibt's Warnungen vom Kernel)

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabio Schmickl");
MODULE_DESCRIPTION("Erstes Kernel-Modul für UE Betriebssysteme");
MODULE_VERSION("1.0");

static int __init demo_init(void)
{
    printk(KERN_INFO "demo module loaded\n");
    return 0;
}

static void __exit demo_exit(void)
{
    printk(KERN_INFO "demo module unloaded\n");
}

module_init(demo_init);
module_exit(demo_exit);
