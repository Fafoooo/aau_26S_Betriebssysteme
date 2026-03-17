#include <stdio.h>
#include <stdlib.h>

static unsigned cnt = 0;

void foo()
{
    char a[2000] = "It works!";

    if (10000 > ++cnt)
    {
        foo();
    }
    else
    {
        printf("%u %s\n", cnt, a);
    }
}


int main(int argc, char** argv)
{
    foo();
    return EXIT_SUCCESS;
}
