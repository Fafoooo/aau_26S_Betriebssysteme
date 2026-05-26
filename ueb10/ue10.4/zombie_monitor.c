// Ü 10.4 - Zombie Monitor Kernel-Modul
//
//
// === Aufgabe ===
//
// Beim Laden durchläuft das Modul einmalig die Prozessliste des Kernels
// und schreibt für jeden Zombie-Prozess Informationen ins Kernel-Log:
// PID, Parent-PID, Befehl, Exit-Status, User-ID, CPU-Zeit (User/Kernel).
//
// Zombies erkennt man an task->exit_state == EXIT_ZOMBIE. Die Navigation
// durch die Task-Liste übernimmt das Makro for_each_process().
//
//
// === Bauen und testen ===
//
//   make
//   ./create_zombie &              # aus 10.3, erzeugt einen Zombie
//   sudo insmod zombie_monitor.ko
//   dmesg | tail                   # Zombie-Infos anzeigen
//   sudo rmmod zombie_monitor
//   make clean

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>   // for_each_process
#include <linux/cred.h>           // task_uid, from_kuid

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabio Schmickl");
MODULE_DESCRIPTION("Listet alle Zombie-Prozesse im Kernel-Log");
MODULE_VERSION("1.0");

static int __init zombie_monitor_init(void)
{
    struct task_struct *task;
    int count = 0;

    printk(KERN_INFO "zombie_monitor: Suche nach Zombie-Prozessen\n");

    // Über alle Prozesse der Task-Liste iterieren.
    for_each_process(task)
    {
        if (task->exit_state == EXIT_ZOMBIE)
        {
            // Hinweis: exit_code enthält den rohen Status. Der eigentliche
            // Exit-Code steht in den oberen 8 Bit ((exit_code >> 8) & 0xFF).
            printk(KERN_INFO
                   "zombie_monitor: PID=%d PPID=%d comm=%s exit_code=%d "
                   "uid=%u utime=%llu stime=%llu\n",
                   task->pid,
                   task->real_parent->pid,
                   task->comm,
                   task->exit_code,
                   from_kuid(&init_user_ns, task_uid(task)),
                   (unsigned long long) task->utime,
                   (unsigned long long) task->stime);
            count++;
        }
    }

    printk(KERN_INFO "zombie_monitor: %d Zombie(s) gefunden\n", count);
    return 0;
}

static void __exit zombie_monitor_exit(void)
{
    printk(KERN_INFO "zombie_monitor: entladen\n");
}

module_init(zombie_monitor_init);
module_exit(zombie_monitor_exit);
