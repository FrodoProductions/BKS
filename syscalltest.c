#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[]) {

  // Ein Puffer für den später eingelesenen Inhalt wird erzeugt (die Größe 100 ist jetzt nur arbiträr gesetzt).
  char puffer[100];


  if (argc < 1) {

    // Nun wird für jeden Input iteriert, damit der Inhalt jeder Datei ausgegeben wird.
    for (size_t i = 0; i < argc-1; i++) {

      // Es wird ein Kanal zur Datei mit Leserechten geöffnet.
      int in = open(argv[i], O_RDONLY);

      // Die Größe des Inhalts der Datei wird ermittelt und dieser dabei in den Puffer geschrieben.
      int inputsize = read(in, &puffer, 1024);

      // Jetzt wird der Puffer in die Konsole ausgegeben.
      write(1, puffer, inputsize);

      // Der Kanal wird geschlossen.
      close(in);
    }

  } else {

    // Es wird ein Eingabeprompt geöffnet, der Inalt in den Puffer geschrieben und dessen Größe gespeichert.
    int inputsize = read(0, &puffer, 1024);

    // Jetzt wird der Puffer in die Konsole ausgegeben.
    write(1, puffer, inputsize);

    // Dahinter kommt eine Newline, damit alles seine Ordnung hat :>
    write(1, "\n", 1);
  }

  return 0;
}