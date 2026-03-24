/* endless.c - infinite loop for ulimit demonstration */

#include <stdio.h>

int main(void)
{
    puts("Running... (use ulimit -t 4 to limit)");
    for (;;);
    return 0;
}
