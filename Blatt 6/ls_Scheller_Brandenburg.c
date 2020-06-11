#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <langinfo.h>
#include <pwd.h>
#include <grp.h>

// Die Filterfunktion für scandir(), welche alle versteckten Dateien und Verzeichnisse ausblendet bzw. nur sichbare zurückgibt.
int selectNonInvisible(const struct dirent *file) {
  // Es wird überprüft, ob es sich bei der ersten Stelle des Namens der Datei um einenn Punkt handelt. 
  return(strncmp(file->d_name, ".", 1));
}

int main(int argc, char const *argv[]) {

  struct dirent **files;
  struct stat statBuffer;

  size_t fileCount;

  char c;

  // Diese Flag gibt an, ob das Programm mit -l aufgerufen wurde und dementsprechend zusätzliche Metadaten ausgegeben werdne sollen.
  int printLong = 0;

  // Dieser Pointer zeigt auf die zu verwendene Filterfunktion. In diesem Programm ist das entwerder selectNonInvisible() oder NULL (Im Fall von -a).
  int (*filter) (const struct dirent*) = &selectNonInvisible;

  const char *path;

  // Falls Argumente übergeben wurde, werden diese hier aufgefangen.
  while((c = getopt (argc, argv, "al")) != -1) {

    switch (c) {
      // Es soll kein Filter angewandt werden, also wird der Pointer auf NULL gesetzt.
      case 'a':
        filter = NULL;
        break;
      case 'l':
        printLong = 1;
        break;
      default:
        printf("ls: illegal option -- %c\nusage: ls [-al] [file ...]\n", optopt);
        return 1;
    }
  }

  // Hier wird überprüft, ob ein Pfad angegeben wurde. Wenn nicht, wird der Pfad auf das aktuelle Verzeichnis (.) gesetzt.
  if (argv[optind] == NULL) {
    path = ".";
  } else if (opendir(argv[optind])) {
    path = argv[optind];
  } else {
    printf("ls: directory either non-existant or access has not been granted.\n");
    return 2;
  }

  // Das Verzeichnis wird gescannt.
  fileCount = scandir(path, &files, (*filter), alphasort);

  //printf("%d files detected in this directory!\n", fileCount);

  for (size_t i = 0; i < fileCount; i++) {

    // Hier werden, falls benötigt, die Metadaten ausgelesen und ausgegeben.
    if (printLong) {

      // Hier wird der vollständige Pfad der aktuellen Datei geildet, da stat diesen bnötigt, wenn Dateien außerhalnb des aktuellen Verzeichnisses
      // betrachtet werden sollen.
      char fullpath[512];
      strcpy(fullpath, path);
      strcat(fullpath, "/");
      strcat(fullpath, files[i]->d_name);

      stat(fullpath, &statBuffer);

      // Der Benutzer- und Gruppenname wird aus der UID bzw. GID der Datei generiert.
      struct passwd *upws = getpwuid(statBuffer.st_uid);
      struct group *gpws = getgrgid(statBuffer.st_gid);
      printf("%s ", upws->pw_name);
      printf("%s ", gpws->gr_name);

      // Das Datum der letzten Modifikation der Datei wird ermittelt. Da ctime grundsätzlich immer eine newline an das Ende der Rückgabe anhängt,
      // wird dies mit strtok() entfernt, damit das Layout etwas übersichtlicher wird.
      printf("%s ", strtok(ctime(&statBuffer.st_mtime), "\n"));
    }

    // Der Name der Datei wird ausgegeben.
    printf("%-s\n", files[i]->d_name);
  }

  return 0;
}
