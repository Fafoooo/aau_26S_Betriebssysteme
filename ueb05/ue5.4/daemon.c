// Ü 5.4 + 5.5 - Daemon mit Signalbehandlung
//
// Was ist ein Daemon?
//   Ein Daemon ist ein Hintergrundprozess der ohne Terminal laeuft.
//   Eigenschaften:
//   - Kein kontrollierendes Terminal (kein stdin/stdout)
//   - Laeuft als Kind von init/systemd (PPID = 1)
//   - Arbeitsverzeichnis ist / (root)
//   - Reagiert auf Signale statt auf Benutzereingaben
//   - Loggt ueber syslog statt printf
//   Beispiele: sshd, httpd, cron
//
// Daemon beenden:
//   kill <PID>          (sendet SIGTERM)
//   kill -9 <PID>       (sendet SIGKILL, nicht abfangbar)
//   Oder: kill -SIGUSR1 <PID> zweimal (unser Signalhandler)
//
// Was ist ein Signal?
//   Ein Signal ist eine asynchrone Benachrichtigung an einen Prozess.
//   Das OS oder ein anderer Prozess sendet es. Der Prozess kann:
//   - Es ignorieren
//   - Einen eigenen Handler ausfuehren (signal() oder sigaction())
//   - Die Standardaktion ausfuehren (z.B. terminieren)
//   Beispiele: SIGTERM (bitte beenden), SIGKILL (sofort beenden),
//   SIGINT (Ctrl+C), SIGUSR1/SIGUSR2 (benutzerdefiniert)
//
// Kompilieren: gcc -o daemon daemon.c
// Starten:     ./daemon
// PID finden:  grep "Daemon gestartet" /var/log/syslog  (oder journalctl)
// Signal:      kill -SIGUSR1 <PID>    (erstes Mal: "received")
//              kill -SIGUSR1 <PID>    (zweites Mal: Daemon beendet sich)

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Zaehlt wie oft SIGUSR1 empfangen wurde
static volatile sig_atomic_t sigusr1_count = 0;

// Signalhandler fuer SIGUSR1
void handle_sigusr1(int sig)
{
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
    // Schritt 1: Fork und Elternprozess beenden
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        // Elternprozess beendet sich -> Kind wird zum Orphan -> adoptiert von init
        printf("Daemon gestartet mit PID %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Schritt 2: Neue Session erstellen (loest sich vom Terminal)
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Schritt 3: Arbeitsverzeichnis auf root setzen
    chdir("/");

    // Schritt 4: stdin/stdout/stderr schliessen
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Schritt 5: Syslog oeffnen
    openlog("mein_daemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon gestartet (PID %d)", getpid());

    // Schritt 6: Signalhandler registrieren
    signal(SIGUSR1, handle_sigusr1);

    // Schritt 7: Endlosschleife (Daemon-Hauptschleife)
    while (1)
    {
        syslog(LOG_INFO, "Daemon laeuft... (PID %d)", getpid());
        sleep(30);  // Alle 30 Sekunden eine Meldung
    }

    closelog();
    return EXIT_SUCCESS;
}
