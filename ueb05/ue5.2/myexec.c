// Ü 5.2 - Minimalistischer Kommando-Interpreter mit exec
//
// Warum execvp?
//   - execvp nimmt ein Array von Strings (wie argv) -> passt perfekt zu unseren Parametern
//   - Das 'p' in execvp sucht das Programm im PATH (wie die Shell)
//   - Das 'v' steht fuer vector (Array), im Gegensatz zu 'l' (Liste einzelner Argumente)
//   - execl/execlp: Man muesste jeden Parameter einzeln angeben -> unflexibel
//   - execve: Braucht den vollen Pfad + Environment-Array -> zu aufwaendig
//
// Aufruf: ./myexec ls -al
// Fuehrt "ls -al" aus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print_usage(const char *prog)
{
    printf("Usage: %s <command> [arguments...]\n", prog);
    printf("Fuehrt das angegebene Kommando aus.\n");
    printf("Beispiel: %s ls -al\n", prog);
}

int main(int argc, char **argv)
{
    // Schritt 0: Argumente pruefen
    if (argc < 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    pid_t pid;

    fprintf(stdout, "[P]\tcurrent process id = %i\n", (int)getpid());

    // Schritt 3.1: Kindprozess erzeugen
    pid = fork();

    if (pid > (pid_t)0)
    {
        // Schritt 3.2: Elternprozess -> auf Kind warten
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            fprintf(stdout, "[P]\tChild exited with status %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            fprintf(stdout, "[P]\tChild killed by signal %d\n", WTERMSIG(status));
        }
    }
    else if (pid == (pid_t)0)
    {
        // Schritt 3.3: Kindprozess -> Programm ausfuehren
        fprintf(stdout, "[C]\tchild process id = %i, executing: %s\n",
                (int)getpid(), argv[1]);

        // argv+1 zeigt auf ["ls", "-al", NULL] -> genau was execvp braucht
        execvp(argv[1], argv + 1);

        // Wenn execvp zurueckkehrt, ist ein Fehler aufgetreten
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Schritt 3.4: fork fehlgeschlagen
        perror("fork failed");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
