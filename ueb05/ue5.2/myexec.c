// Ü 5.2 - Minimalistischer Kommando-Interpreter mit exec
//
//
// === Aufgabe ===
//
// Das Programm nimmt einen Kommandonamen und dessen Parameter entgegen
// und führt den Befehl mit einer Funktion der exec-Familie aus.
//
// Aufruf:  ./myexec ls -al
// Wirkung: innerhalb von myexec wird "ls -al" ausgeführt.
//
//
// === Welche exec-Variante ist am geeignetsten? ===
//
// Wir verwenden execvp, weil:
//   - das 'v' ein Array von Strings (vector) erwartet. Das passt genau
//     zum Format von argv, ohne dass wir etwas umbauen müssen.
//   - das 'p' das Programm im PATH sucht, wie es auch die Shell tut.
//     Man muss keinen vollen Pfad wie /bin/ls angeben.
//
// Die anderen Varianten sind für diesen Fall weniger praktisch:
//   - execl / execlp: erwarten die Argumente einzeln aufgezählt.
//     Bei beliebig vielen Parametern nicht praktikabel.
//   - execve / execvpe: brauchen zusätzlich ein Environment-Array,
//     hier nicht nötig.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "[P] current process id = %d\n", (int)getpid());

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        // Kindprozess: das übergebene Kommando ausführen.
        // argv+1 zeigt auf ["ls", "-al", ..., NULL] -- genau das Format,
        // das execvp erwartet.
        fprintf(stdout, "[C] child process id = %d, executing: %s\n",
                (int)getpid(), argv[1]);

        execvp(argv[1], argv + 1);

        // Wenn execvp zurückkehrt, ist ein Fehler aufgetreten.
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Elternprozess: auf das Kind warten und Status ausgeben.
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        fprintf(stdout, "[P] Child exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        fprintf(stdout, "[P] Child killed by signal %d\n", WTERMSIG(status));
    }

    return EXIT_SUCCESS;
}
