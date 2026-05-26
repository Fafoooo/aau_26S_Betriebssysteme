// Ü 10.3 - Zombie-Prozess erzeugen
//
// Das Kind beendet sich sofort. Der Elternprozess ruft absichtlich
// kein wait() auf und läuft 5 Minuten weiter -- in dieser Zeit bleibt
// das beendete Kind ein Zombie.
//
//   gcc create_zombie.c -o create_zombie
//   ./create_zombie &
//   ps aux | grep 'Z'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Kind: beendet sich sofort.
        printf("Kind (PID %d) beendet sich jetzt.\n", getpid());
        exit(EXIT_SUCCESS);
    }

    // Eltern: KEIN wait() -> das beendete Kind wird zum Zombie.
    printf("Eltern (PID %d): Kind %d ist jetzt ein Zombie.\n", getpid(), pid);
    printf("Pruefen mit: ps aux | grep 'Z'\n");

    // Mindestens 5 Minuten weiterlaufen.
    sleep(300);

    printf("Eltern beendet sich.\n");
    return EXIT_SUCCESS;
}
