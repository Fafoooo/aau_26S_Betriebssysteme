// Ue 6.5 - pipe + dup2
//
// Aufruf:
//   ./myduppipe "ls -al" "sort -r"
//
// Entspricht:
//   ls -al | sort -r
//
// Das Programm startet zwei Kindprozesse. Das erste Kind schreibt seine
// Standardausgabe in eine Pipe. Das zweite Kind bekommt diese Pipe als
// Standardeingabe. Beide Kommandos laufen ueber /bin/bash -c, damit
// Argumente wie "ls -al" als ein Shell-Kommando funktionieren.
//
// Kompilieren:
//   gcc -o myduppipe myduppipe.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FULLBASH "/bin/bash"
#define BASH "bash"
#define BASHOPTION "-c"

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("USAGE: %s \"<cmd1>\" \"<cmd2>\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thePipe[2];
    if (pipe(thePipe) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid_c1 = fork();
    if (pid_c1 == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid_c1 == 0)
    {
        // Erstes Kind: stdout -> Pipe
        close(thePipe[READ]);
        dup2(thePipe[WRITE], STDOUT_FILENO);
        close(thePipe[WRITE]);

        execl(FULLBASH, BASH, BASHOPTION, argv[1], (char *)NULL);
        perror("execl cmd1");
        exit(EXIT_FAILURE);
    }

    pid_t pid_c2 = fork();
    if (pid_c2 == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid_c2 == 0)
    {
        // Zweites Kind: Pipe -> stdin
        close(thePipe[WRITE]);
        dup2(thePipe[READ], STDIN_FILENO);
        close(thePipe[READ]);

        execl(FULLBASH, BASH, BASHOPTION, argv[2], (char *)NULL);
        perror("execl cmd2");
        exit(EXIT_FAILURE);
    }

    // Elternprozess benutzt die Pipe selbst nicht.
    close(thePipe[READ]);
    close(thePipe[WRITE]);

    waitpid(pid_c1, NULL, 0);
    waitpid(pid_c2, NULL, 0);

    return EXIT_SUCCESS;
}
