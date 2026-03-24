#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* define preprocessor macros */
#define READ 0
#define WRITE 1


int main(int argc, char** argv)
{
    /* ---------------------------- */
    /* step 1: create pipe          */
    int   thePipe[2];
    pid_t pid;

    /* TODO pipe() and check whether is was successful */

    /* ----------------------------- */
    /* step 2: fork stuff & co       */
    pid = fork();
    if (pid < (pid_t) 0)
    {
        /* step 4.1: fork failed */
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }

    if (pid == (pid_t) 0)
    {
        /* step 3: code for child */
        /* TODO close unnecessary pipes and create local variables */

        /* TODO Generate random values */

        /* TODO write int values */
        /*      use sleep(rand() % 5) for better readability on display */

        /* TODO close pipes and exit */
    }

    /* step 4: code for parent */

    /* TODO close unnecessary pipes */

    /* TODO Read int values from child */

    /* TODO calculate result and display the result on the screen */

    /* TODO close pipes */

    /* TODO wait for child and exit*/
};
