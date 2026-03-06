#include <stdio.h>

int is_prime(int n) {
    if (n < 2)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;
    for (int i = 3; i< n; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(void) {
    for (int i = 0; i <= 1000; i++) {
        printf("%d", i);
        if (i % 2 == 0)
            printf("x");
        if (is_prime(i))
            printf("p");
        printf("\n");
    }
    return 0;
}
