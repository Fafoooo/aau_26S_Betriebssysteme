// Ü 6.3 - Wie 6.2, aber mit Shared Memory statt Datei
//
// Identische Logik wie abwechselnd.c, nur statt data.txt liegt der
// Text in einem System-V-Shared-Memory-Segment, auf das beide
// Prozesse zugreifen.
//
// Schritte:
//   shmget()  Segment im Kernel anlegen
//   shmat()   Segment in den eigenen Adressraum mappen
//   shmdt()   Mapping wieder lösen
//   shmctl(IPC_RMID)  Segment endgültig löschen (sonst Leak)
//
// Synchronisation: wieder zwei Pipes als Token-Kanäle.
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

// String in-place umdrehen: ABC -> CBA
static void reverse(char *s, size_t n)
{
    for (size_t i = 0; i < n / 2; i++) {  // Zeichen vorne+hinten tauschen
        char t = s[i];                    // Anfang merken
        s[i] = s[n - 1 - i];              // Ende nach vorne
        s[n - 1 - i] = t;                 // gemerkten Anfang nach hinten
    }
}

int main(void)
{
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666 | IPC_CREAT);  // Kernel-Segment anlegen
    if (shmid == -1) { perror("shmget"); exit(EXIT_FAILURE); } // Fehlerbehandlung

    int to_child[2], to_parent[2];        // 2 Pipes für Token-Sync
    pipe(to_child);                        // Eltern->Kind  ("du bist dran")
    pipe(to_parent);                       // Kind->Eltern  ("du bist dran")

    if (fork() == 0) {                     // 0 = wir sind im Kind
        // === Kind ===
        close(to_child[1]); close(to_parent[0]);   // unbenutzte Pipe-Enden zu
        char *shm = shmat(shmid, NULL, 0); // SHM in eigenen Adressraum mappen mit array 
        char tok;                          // Token-Byte für Pipe

        for (int i = 0; i < ITER; i++) {   // 5 Durchläufe
            read(to_child[0], &tok, 1);    // warten bis Eltern schickt
            reverse(shm, strlen(shm));     // Text im SHM direkt umdrehen
            write(to_parent[1], "x", 1);   // Eltern wecken
        }

        shmdt(shm);                        // SHM unmappen
        exit(EXIT_SUCCESS);                // Kind sauber beenden
    }

    // === Eltern ===
    close(to_child[0]); close(to_parent[1]);  // unbenutzte Pipe-Enden zu
    char *shm = shmat(shmid, NULL, 0);     // SHM in eigenen Adressraum mappen
    char text[64], tok;                    // Text-Buffer + Token-Byte

    for (int i = 0; i < ITER; i++) {       // 5 Durchläufe
        snprintf(text, sizeof(text),       // Text für diese Iteration bauen
                 "Iteration %d: HelloWorld - genug Zeichen!", i + 1);
        strncpy(shm, text, SHM_SIZE);      // Text ins SHM schreiben
        printf("[Eltern %d] schrieb:    \"%s\"\n", i + 1, shm);  // Log

        write(to_child[1], "x", 1);        // Kind wecken
        read(to_parent[0], &tok, 1);       // warten bis Kind fertig

        printf("[Eltern %d] invertiert: \"%s\"\n", i + 1, shm);  // Log
        shm[0] = 0;                        // SHM "leeren" (wie unlink in 6.2)
    }

    wait(NULL);                            // auf Kind warten (kein Zombie)
    shmdt(shm);                            // SHM unmappen
    shmctl(shmid, IPC_RMID, NULL);         // Segment im Kernel löschen, sonst Leak
    return EXIT_SUCCESS;
}
