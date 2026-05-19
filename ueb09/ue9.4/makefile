# Ü 9.4 - Makefile mit Kommentaren
#
#
# === Wofür ist ein Makefile? ===
#
# Ein Makefile beschreibt, wie ein Projekt aus mehreren Quelldateien
# zusammengebaut wird. Der make-Befehl liest die Regeln und führt
# nur die Schritte aus, die wirklich nötig sind. Wurde z.B. nur eine
# Quelldatei geändert, wird auch nur diese neu kompiliert.
#
#
# === Warum funktioniert make im Original NICHT? ===
#
# Die OBJ-Liste verlangt add.o. make leitet aus der Pattern-Regel
# "%.o: %.c" ab, dass add.o aus add.c gebaut werden muss. Im Angabe-
# Paket fehlt aber add.c -- nur die Header-Datei add.h ist da. Daher
# bricht make ab mit:
#
#   make: *** No rule to make target 'add.c', needed by 'add.o'. Stop.
#
# Fix: add.c anlegen, in der die Funktion int add(int, int) tatsächlich
# implementiert wird. Mit der hinzugefügten add.c läuft make ohne
# Änderungen am Makefile.
#
#
# === Verwendung ===
#
#   make           baut das Programm (myprogram)
#   ./myprogram    führt es aus, gibt "The result is 30" aus
#   make clean     löscht alle erzeugten Dateien (.o und myprogram)


# Compiler-Variable. Mit "make CC=clang" lässt sich der Compiler überschreiben.
CC=gcc

# Compiler-Flags. -I. fügt das aktuelle Verzeichnis zum Include-Pfad hinzu,
# damit #include "add.h" gefunden wird.
CFLAGS=-I.

# Liste der Objektdateien, die für myprogram nötig sind.
OBJ = main.o add.o

# Hauptziel: aus den Objektdateien wird das Programm "myprogram" gelinkt.
#   $@ steht für das Ziel (myprogram)
#   $^ steht für alle Abhängigkeiten (main.o add.o)
myprogram: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Pattern-Regel: aus jeder .c-Datei wird die gleichnamige .o-Datei.
#   -c   bedeutet "nur kompilieren, nicht linken"
#   $@   ist die Ziel-.o-Datei
#   $<   ist die erste Abhängigkeit (also die passende .c-Datei)
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Aufräumen: löscht alle erzeugten Dateien.
clean:
	rm -f $(OBJ) myprogram
