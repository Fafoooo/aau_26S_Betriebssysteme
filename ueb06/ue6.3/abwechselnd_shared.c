// Ü 6.3 - Wie 6.2, aber mit Shared Memory statt Datei
//
// Identische Logik wie abwechselnd.c, nur statt data.txt liegt der
// Text in einem System-V-Shared-Memory-Segment, auf das beide
// Prozesse zugreifen.
//
// Schritte:
//   shmget()  Segment im Kernel anlegen
//   shmat()   Segment in den eigenen Adressraum mappen
//   shmdt()   Mapping wieder loesen
//   shmctl(IPC_RMID)  Segment endgueltig loeschen (sonst Leak)
//
// Synchronisation: wieder zwei Pipes als Token-Kanaele.
//
// Kompilieren:  gcc -o abwechselnd_shared abwechselnd_shared.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define ITER 5
#define SHM_SIZE 256

static void reverse(char *s, size_t n)
{
    for (size_t i = 0; i < n / 2; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
}

int main(void)
{
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) { perror("shmget"); exit(EXIT_FAILURE); }

    int to_child[2], to_parent[2];
    pipe(to_child);
    pipe(to_parent);

    if (fork() == 0) {
        // === Kind ===
        close(to_child[1]); close(to_parent[0]);
        char *shm = shmat(shmid, NULL, 0);
        char tok;

        for (int i = 0; i < ITER; i++) {
            read(to_child[0], &tok, 1);
            reverse(shm, strlen(shm));
            write(to_parent[1], "x", 1);
        }

        shmdt(shm);
        exit(EXIT_SUCCESS);
    }

    // === Eltern ===
    close(to_child[0]); close(to_parent[1]);
    char *shm = shmat(shmid, NULL, 0);
    char text[64], tok;

    for (int i = 0; i < ITER; i++) {
        snprintf(text, sizeof(text),
                 "Iteration %d: HelloWorld - genug Zeichen!", i + 1);
        strncpy(shm, text, SHM_SIZE);
        printf("[Eltern %d] schrieb:    \"%s\"\n", i + 1, shm);

        write(to_child[1], "x", 1);
        read(to_parent[0], &tok, 1);

        printf("[Eltern %d] invertiert: \"%s\"\n", i + 1, shm);
        shm[0] = 0;  // Inhalt zuruecksetzen (wie unlink in 6.2)
    }

    wait(NULL);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);   // Segment loeschen, sonst Leak
    return EXIT_SUCCESS;
}
