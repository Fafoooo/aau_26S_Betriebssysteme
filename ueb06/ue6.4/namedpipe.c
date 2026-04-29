// Ü 6.4 - Named Pipe (FIFO) Client-Server
//
// Eine FIFO ist eine "named" Pipe - sie hat einen Pfad im Dateisystem
// (hier /tmp/myfifo) und kann von völlig unverwandten Prozessen
// genutzt werden. Im Gegensatz zu anonymen Pipes muss kein gemeinsamer
// Vorfahre existieren.
//
// Hier zur Demo: Server und Client als Eltern + Kind in einem Programm.
//
// Wichtig: open(O_WRONLY) und open(O_RDONLY) auf einer FIFO blockieren
// beide, bis das jeweils andere Ende geöffnet wird. Das ist der
// Sync-Mechanismus.
//
// Kompilieren:  gcc -o namedpipe namedpipe.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/myfifo"

int main(void)
{
    // FIFO im Dateisystem anlegen. EEXIST = OK, war schon da.
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {  // FIFO erzeugen
        perror("mkfifo");                                      // Fehler-Log
        exit(EXIT_FAILURE);                                    // Abbruch
    }

    if (fork() == 0) {                                          // 0 = wir sind im Kind
        // === Client: schreibt ===
        int fd = open(FIFO_PATH, O_WRONLY);                     // FIFO zum Schreiben öffnen (blockt bis Server liest)
        const char *msg = "Hello server, das ist die Nachricht vom Client!";  // Nachricht
        write(fd, msg, strlen(msg) + 1);                        // Nachricht in FIFO schreiben
        close(fd);                                              // FIFO schließen
        printf("[Client] hat gesendet: \"%s\"\n", msg);         // Log
        exit(EXIT_SUCCESS);                                     // Client beenden
    }

    // === Server: liest ===
    int fd = open(FIFO_PATH, O_RDONLY);                         // FIFO zum Lesen öffnen (blockt bis Client schreibt)
    char buf[256];                                              // Empfangs-Buffer
    ssize_t n = read(fd, buf, sizeof(buf) - 1);                 // Nachricht aus FIFO lesen
    buf[n] = 0;                                                 // Null-Terminator setzen
    printf("[Server] hat empfangen: \"%s\"\n", buf);            // Log
    close(fd);                                                  // FIFO schließen

    wait(NULL);                                                 // auf Client warten (kein Zombie)
    unlink(FIFO_PATH);                                          // FIFO aus Dateisystem entfernen
    return EXIT_SUCCESS;
}
