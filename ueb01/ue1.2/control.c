#include <stdio.h>
#include <string.h>

#define MAX 1001

int main(void) {
    int is_prime[MAX];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (int i = 2; i * i < MAX; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < MAX; j += i)
                is_prime[j] = 0;
        }
    }

    for (int i = 0; i < MAX; i++) {
        printf("%d", i);
        if (i % 2 == 0)
            printf("x");
        if (is_prime[i])
            printf("p");
        printf("\n");
    }
    return 0;
}
