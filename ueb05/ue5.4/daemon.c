// Ü 5.4 + 5.5 - Daemon mit Signalbehandlung
//
//
// === Was ist ein Daemon? ===
//
// Ein Daemon ist ein Hintergrundprozess, der ohne Terminal läuft.
// Typische Eigenschaften:
//   - kein kontrollierendes Terminal (kein stdin/stdout/stderr)
//   - läuft als Kind von init oder systemd (Parent-PID = 1)
//   - Arbeitsverzeichnis ist / (root)
//   - loggt über syslog statt über printf
//   - reagiert auf Signale statt auf Benutzereingaben
//
// Beispiele: sshd, httpd, cron.
//
//
// === Was ist ein Signal? ===
//
// Ein Signal ist eine asynchrone Benachrichtigung an einen Prozess.
// Das OS oder ein anderer Prozess schickt es. Der empfangende Prozess
// kann das Signal ignorieren, einen eigenen Handler ausführen oder die
// Standardaktion machen (zum Beispiel terminieren).
//
// Beispiele:
//   - SIGTERM     bitte beenden (Standard von kill)
//   - SIGKILL     sofort beenden, nicht abfangbar
//   - SIGINT      Ctrl+C vom Terminal
//   - SIGUSR1/2   benutzerdefiniert, für eigene Zwecke
//
//
// === Wie beendet man den Daemon? ===
//
//   kill <PID>                # SIGTERM (Standard)
//   kill -9 <PID>             # SIGKILL (nicht abfangbar)
//   kill -SIGUSR1 <PID>       # zweimal senden -> unser Handler beendet ihn
//
//
// === Ablauf in diesem Programm ===
//
//   1. fork: Elternprozess beendet sich, Kind läuft weiter und wird
//      von init adoptiert.
//   2. setsid: neue Session, das Kind löst sich vom Terminal.
//   3. chdir("/"): Arbeitsverzeichnis auf root setzen.
//   4. stdin/stdout/stderr schließen.
//   5. syslog öffnen und Startmeldung schreiben.
//   6. Signalhandler für SIGUSR1 registrieren.
//   7. Endlosschleife mit regelmäßigen Lebenszeichen.
//
//
// === Kompilieren und laufen lassen ===
//
//   gcc -o daemon daemon.c
//   ./daemon                             # PID wird auf stdout ausgegeben
//   journalctl -f | grep mein_daemon     # Log live verfolgen
//   kill -SIGUSR1 <PID>                  # 1. Mal: "SIGUSR1 received"
//   kill -SIGUSR1 <PID>                  # 2. Mal: Daemon beendet sich

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Zählt, wie oft SIGUSR1 empfangen wurde.
static volatile sig_atomic_t sigusr1_count = 0;

void handle_sigusr1(int sig)
{
    (void)sig;
    sigusr1_count++;

    if (sigusr1_count == 1)
    {
        syslog(LOG_INFO, "SIGUSR1 received");
    }
    else
    {
        syslog(LOG_INFO, "SIGUSR1 received again -- shutting down");
        syslog(LOG_INFO, "Daemon beendet (PID %d)", getpid());
        closelog();
        exit(EXIT_SUCCESS);
    }
}

int main(void)
{
    // fork: Eltern beenden sich, Kind läuft als Daemon weiter.
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        printf("Daemon gestartet mit PID %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Neue Session, löst sich vom Terminal.
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Arbeitsverzeichnis auf root setzen und Standard-IO schließen.
    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Syslog öffnen und Start melden.
    openlog("mein_daemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon gestartet (PID %d)", getpid());

    // Signalhandler für SIGUSR1 registrieren.
    signal(SIGUSR1, handle_sigusr1);

    // Hauptschleife: alle 30 Sekunden ein Lebenszeichen ins Log.
    while (1)
    {
        syslog(LOG_INFO, "Daemon laeuft... (PID %d)", getpid());
        sleep(30);
    }

    closelog();
    return EXIT_SUCCESS;
}
