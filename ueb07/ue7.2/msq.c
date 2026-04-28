// Ü 7.2 - IPC mit System-V Message Queues
//
// Eltern + Kind tauschen eine Nachricht über eine Message Queue aus.
// Kind sendet, Eltern empfängt. Danach wird die Queue mit
// msgctl(IPC_RMID) aus dem Kernel gelöscht (sonst Leak).
//
// Beispielausgabe:
//   sending <Hello parent from child using messageq> in 1596
//   received <Hello parent from child using messageq> in 1592
//
// Kompilieren:  gcc -o msq msq.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

// Aus dem Aufgabenblatt vorgegeben
struct message {
    long mtype;
    char mtext[8192];
};

int main(void)
{
    // Neue, anonyme Queue im Kernel anlegen
    int qid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (qid == -1) { perror("msgget"); exit(EXIT_FAILURE); }

    if (fork() == 0)
    {
        // === Kind: sendet ===
        struct message msg = { .mtype = 1 };
        strncpy(msg.mtext,
                "Hello parent from child using messageq",
                sizeof(msg.mtext));

        printf("sending <%s> in %d\n", msg.mtext, getpid());

        // Größe = nur Nutzdaten-Länge (ohne mtype)
        msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0);
        exit(EXIT_SUCCESS);
    }

    // === Eltern: empfängt ===
    struct message msg;
    msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0);
    printf("received <%s> in %d\n", msg.mtext, getpid());

    // Kind einsammeln und Queue löschen
    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
