// Ü 4.2 - Erzeugung von Prozessen mit fork()
//
//
// === Rückgabewerte von fork() ===
//
// Rückgabetyp: pid_t (definiert in <sys/types.h>)
//
//   - Im Elternprozess: PID des neuen Kindes (größer als 0)
//   - Im Kindprozess:   0
//   - Bei Fehler:      -1
//
//
// === Ablauf des Codefragments ===
//
// Die äußere Schleife läuft zweimal und forkt jedes Mal einmal. Das
// Kind des äußeren fork betritt den if-Block und führt die innere
// Schleife aus. Nach der inneren Schleife fällt das Kind zurück in
// die äußere Schleife und forkt bei der nächsten Iteration erneut.
//
// Die innere Schleife läuft ebenfalls zweimal und forkt jedes Mal.
// Das Kind des inneren fork beendet sich sofort mit exit(0) und
// erzeugt keine weiteren Prozesse.
//
// Das Wichtigste dabei:
//
//   - Kinder des ÄUSSEREN fork leben weiter und können selbst forken.
//   - Kinder des INNEREN fork sind Einweg-Prozesse (sofort exit).
//
//
// === Der Prozessbaum ===
//
//   P (Original, Hauptprozess)
//   |
//   +-- C1   (erzeugt bei P.i=0, äußerer fork)
//   |   |
//   |   +-- C1a    (innerer fork bei C1.j=0, exit)
//   |   +-- C1b    (innerer fork bei C1.j=1, exit)
//   |   +-- C1c    (äußerer fork bei C1.i=1)
//   |       |
//   |       +-- C1c_a    (innerer fork bei C1c.j=0, exit)
//   |       +-- C1c_b    (innerer fork bei C1c.j=1, exit)
//   |
//   +-- C2   (erzeugt bei P.i=1, äußerer fork)
//       |
//       +-- C2a    (innerer fork bei C2.j=0, exit)
//       +-- C2b    (innerer fork bei C2.j=1, exit)
//
// Insgesamt werden neben P 9 weitere Kindprozesse erzeugt.
// Zusammen mit P sind das 10 Prozesse.
//
// Nicht vergessen: C1 forkt zweimal. Einmal die beiden inneren Kinder
// C1a und C1b, und danach, bei der nächsten Iteration der äußeren
// Schleife (C1.i=1), noch einmal C1c. C1c macht dann selbst wieder
// die innere Schleife.
//
//
// === Beobachtung mit ps/pstree ===
//
// Während das Programm läuft (sleep(2) gibt genug Zeit), kann man
// in einem zweiten Terminal den realen Prozessbaum anschauen:
//
//     pstree -p $(pgrep forktree)
//     ps -ef | grep forktree

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    for (int i = 0; i < 2; i++)
    {
        if (fork() == 0)
        {
            printf("Outer Hello World (PID=%d, PPID=%d)\n",
                   getpid(), getppid());

            for (int j = 0; j < 2; j++)
            {
                pid = fork();
                printf("-- Inner Hello World (PID=%d, PPID=%d)\n",
                       getpid(), getppid());

                if (pid == 0)
                {
                    // Kindprozess des inneren fork: sofort beenden
                    exit(0);
                }
            }
        }
    }

    // Pause, damit man in einem zweiten Terminal Zeit hat, den
    // Prozessbaum mit ps oder pstree anzuschauen.
    sleep(10);

    // Auf alle direkten Kinder warten, damit keine Zombies übrig bleiben.
    while (wait(NULL) > 0);

    return EXIT_SUCCESS;
}
