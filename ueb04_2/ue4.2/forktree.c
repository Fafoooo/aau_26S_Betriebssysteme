// Übung 4.2 - Prozesserzeugung mit fork()
//
//
// == Rückgabewerte von fork() ==
//
// fork() gibt einen Wert vom Typ pid_t zurück (aus <sys/types.h>):
//
//   - Elternprozess erhält: PID des erzeugten Kindes (> 0)
//   - Kindprozess erhält:   0
//   - Fehlerfall:          -1
//
//
// == Ablauf des Programms ==
//
// Es gibt eine äußere Schleife (2 Durchläufe) und eine innere (2 Durchläufe).
// Bei jedem Durchlauf der äußeren Schleife wird ein fork() aufgerufen.
// Das erzeugte Kind geht in den if-Block und führt dort die innere
// Schleife aus. In der inneren Schleife wird nochmals geforkt, aber
// diese Kinder beenden sich direkt über exit(0).
//
// Nach der inneren Schleife fällt das Kind der äußeren Schleife zurück
// und kann bei der nächsten Iteration erneut forken.
//
// Wichtig:
//   - Kinder der äußeren Schleife existieren weiter und forken selbst.
//   - Kinder der inneren Schleife beenden sich sofort.
//
//
// == Prozessbaum ==
//
//   P (Hauptprozess)
//   |
//   +-- K1   (äußerer fork, i=0)
//   |   |
//   |   +-- K1_x   (innerer fork, j=0, beendet sich)
//   |   +-- K1_y   (innerer fork, j=1, beendet sich)
//   |   +-- K1_z   (äußerer fork, i=1)
//   |       |
//   |       +-- K1_z_x   (innerer fork, j=0, beendet sich)
//   |       +-- K1_z_y   (innerer fork, j=1, beendet sich)
//   |
//   +-- K2   (äußerer fork, i=1)
//       |
//       +-- K2_x   (innerer fork, j=0, beendet sich)
//       +-- K2_y   (innerer fork, j=1, beendet sich)
//
// Insgesamt: 10 Prozesse (P + 9 erzeugte Kinder).
//
// Hinweis: K1 erzeugt nicht nur die inneren Kinder K1_x und K1_y,
// sondern auch K1_z in der nächsten Iteration der äußeren Schleife.
//
//
// == Überprüfung ==
//
// Während das Programm läuft (sleep gibt Zeit), kann man den Baum
// im Terminal ansehen:
//     pstree -p $(pgrep forktree)
//     ps -ef | grep forktree

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child_pid;

    for (int i = 0; i < 2; i++)
    {
        if (fork() == 0)
        {
            printf("Äußeres fork (PID=%d, Eltern-PID=%d)\n",
                   getpid(), getppid());

            for (int j = 0; j < 2; j++)
            {
                child_pid = fork();
                printf("  Inneres fork (PID=%d, Eltern-PID=%d)\n",
                       getpid(), getppid());

                if (child_pid == 0)
                {
                    exit(0);
                }
            }
        }
    }

    sleep(10);

    while (wait(NULL) > 0);

    return EXIT_SUCCESS;
}
