// Ü 6.4 - Named Pipe (FIFO) Client-Server
//
// Eine FIFO ist eine "named" Pipe - sie hat einen Pfad im Dateisystem
// (hier /tmp/myfifo) und kann von voellig unverwandten Prozessen
// genutzt werden. Im Gegensatz zu anonymen Pipes muss kein gemeinsamer
// Vorfahre existieren.
//
// Hier zur Demo: Server und Client als Eltern + Kind in einem Programm.
//
// Wichtig: open(O_WRONLY) und open(O_RDONLY) auf einer FIFO blockieren
// beide, bis das jeweils andere Ende geoeffnet wird. Das ist der
// Sync-Mechanismus.
//
// Kompilieren:  gcc -o namedpipe namedpipe.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/myfifo"

int main(void)
{
    // FIFO im Dateisystem anlegen. EEXIST = OK, war schon da.
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    if (fork() == 0) {
        // === Client: schreibt ===
        int fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) { perror("open client"); exit(EXIT_FAILURE); }

        const char *msg = "Hello server, das ist die Nachricht vom Client!";
        write(fd, msg, strlen(msg) + 1);
        close(fd);
        printf("[Client] hat gesendet: \"%s\"\n", msg);
        exit(EXIT_SUCCESS);
    }

    // === Server: liest ===
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) { perror("open server"); return EXIT_FAILURE; }

    char buf[256];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    if (n == -1) { perror("read"); return EXIT_FAILURE; }

    buf[n] = 0;
    printf("[Server] hat empfangen: \"%s\"\n", buf);
    close(fd);

    wait(NULL);
    unlink(FIFO_PATH);   // FIFO aus dem Dateisystem entfernen
    return EXIT_SUCCESS;
}
