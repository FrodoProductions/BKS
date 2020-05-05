#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[]) {

  // Ein Puffer für den später eingelesenen Inhalt wird erzeugt (die Größe 100 ist jetzt nur arbiträr gesetzt).
  char puffer[100];

  // Es wird überprüft, ob es sich bei dem Input um eine Datei handelt (bzw. es wird geguckt, ob eine Datei unter dem Pfad existiert).
  // Diese Art der Verwendung von access() hab ich in diesem StackOverflow-Thread gefunden:
  // https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
  if (access(*++argv, F_OK) == 0) {

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

    // Nun wird für jeden Input iteriert, damit jeder Input ausgegeben wird.
    for (size_t i = 0; i < argc-1; i++) {

      // Der String wird in die Konsole ausgegeben.
      write(1, argv[i], strlen(argv[i]));

      // Dahinter kommt eine Newline, damit alles eine Ordnung hat :>
      write(1, "\n", 1);
    }
  }

  return 0;
}
