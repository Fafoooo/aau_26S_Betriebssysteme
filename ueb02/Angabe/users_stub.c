#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXCHARS (200 + 1)  // we assume a line in the file has a maximum length of 200 characters + \0


int main(int argc, char* argv[])
{
    FILE* file = fopen("/etc/passwd", "r");  // open file

    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

    // TODO:
    // 1. read each line from from file
    // 2. parse for username (first column) and print
    // 3. find third column and print user-id

    // TODO: close file

    return 0;
}
