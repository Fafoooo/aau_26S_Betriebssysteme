// Ü 4.2 - Erzeugung von Prozessen mit fork()
//
// fork() gibt zurueck:
//   - pid_t (Typ: int-artig, definiert in sys/types.h)
//   - Im Elternprozess: PID des Kindes (> 0)
//   - Im Kindprozess: 0
//   - Bei Fehler: -1
//
// Prozessbaum fuer den gegebenen Code:
//
// P (Original)
// ├── C1 (i=0, fork()==0)
// │   ├── C1a (j=0, inner fork Kind)  -> exit(0)
// │   └── C1b (j=1, inner fork Kind)  -> exit(0)
// │   (C1 selbst: nach inner loop, faehrt mit i=1 fort)
// │   └── C1c (i=1, fork()==0)
// │       ├── C1c_a (j=0, inner fork Kind) -> exit(0)
// │       └── C1c_b (j=1, inner fork Kind) -> exit(0)
// └── C2 (i=1, fork()==0)
//     ├── C2a (j=0, inner fork Kind) -> exit(0)
//     └── C2b (j=1, inner fork Kind) -> exit(0)
//
// Insgesamt werden neben P noch 8 Kindprozesse erzeugt (9 Prozesse total).
// Erklärung:
//   - Iteration i=0: P forkt C1. C1 druckt "Outer Hello World"
//     Inner loop j=0: C1 forkt C1a. C1a druckt "Inner Hello World" und exit(0).
//                     C1 druckt auch "Inner Hello World" (parent-Seite von inner fork).
//     Inner loop j=1: C1 forkt C1b. C1b druckt "Inner Hello World" und exit(0).
//                     C1 druckt auch "Inner Hello World".
//     C1 geht weiter zur naechsten Iteration i=1.
//   - Iteration i=1 fuer P: P forkt C2. Gleiche Logik wie C1.
//   - Iteration i=1 fuer C1: C1 forkt C1c. C1c druckt "Outer Hello World"
//     und macht ebenfalls den inner loop (erzeugt C1c_a und C1c_b).

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("[P] Hauptprozess PID = %d\n", getpid());

    for (int i = 0; i < 2; i++)
    {
        if (fork() == 0)
        {
            printf("[Kind] Outer Hello World (PID=%d, Parent=%d, i=%d)\n",
                   getpid(), getppid(), i);

            for (int j = 0; j < 2; j++)
            {
                pid = fork();
                printf("[%s] -- Inner Hello World (PID=%d, Parent=%d, j=%d)\n",
                       pid == 0 ? "Enkel" : "Kind", getpid(), getppid(), j);

                if (pid == 0)
                {
                    // Kindprozess des inneren fork -> sofort beenden
                    exit(0);
                }
            }
            // Kindprozess des aeusseren fork -> nach inner loop beenden
            // (ausser es gibt noch eine i-Iteration, dann laeuft es weiter)
        }
    }

    // Elternprozess: auf alle Kinder warten
    sleep(2);  // Kurze Pause damit die Ausgabe lesbar ist
    while (wait(NULL) > 0);

    printf("[P] Alle Kinder beendet. PID = %d\n", getpid());

    return EXIT_SUCCESS;
}
