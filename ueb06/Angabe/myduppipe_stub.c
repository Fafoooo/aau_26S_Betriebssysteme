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


int main(int argc, char** argv)
{
    /* ----------------------------- */
    /* step 0: check input arguments */
    if (argc < 3 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("\nUSAGE: %s, \"<cmd1>\" \"<cmd2>\"\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* ----------------------------- */
    /* step 1: pipes                 */
    int thePipe[2]; /* pipe:  */

    // TODO

    /* ----------------------------- */
    /* step 2: fork stuff & co       */
    pid_t pid_c1, pid_c2;
    pid_c1 = fork();

    // TODO

    if (pid_c1 == (pid_t) 0)
    {
        /* code for first child */
        // TODO
    }

    pid_c2 = fork();

    // TODO

    if (pid_c2 == (pid_t) 0)
    {
        /* code for second child */
        // TODO
    }

    /* code for parent */
    // TODO

    return EXIT_SUCCESS;
}
