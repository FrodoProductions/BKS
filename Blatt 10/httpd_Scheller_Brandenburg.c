#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Der Compiler gibt eine Warnung aus, wenn ich versuche, Rückgabewerte wie -1 mit x < 0 abzufangen. Dies geschieht mit der Begründung, dass x unsigned ist,
// was im Fall einer fehlerfreien Ausführung sicherlich der Fall ist, abfangen muss ich die anderen Fälle aber trotzdem. Ich hoffe, man kann mir für diese unschönen Ausgaben im Compiler verzeihen.

// Quelle für die Anwendung von select(): https://www.gnu.org/software/libc/manual/html_node/Server-Example.html (25.06.2020, 23:04 Uhr)

int main(int argc, char const *argv[]) {
  unsigned int s, s2;
  struct sockaddr_in local, remote;
  fd_set active_fds, read_fds, write_fds;
  int len, i, status;
  char buf[512], filename[512];

  // Es wird überprüft, ob eine Addresse und ein Port angegeben wurden.
  if (argc != 3) {
    perror("Please provide an address and a port.");
    exit(1);
  }

  // Ein Stream-Socket wird geöffnet.
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("Could not create socket");
    exit(1);
  }

  // Addresse und Port des Sockets werden auf die übergebenen Werte eingestellt.
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = inet_addr(argv[1]);
  local.sin_port = htons(atoi(argv[2]));

  // Der Socket wird als reusable eingestuft, damit er nach seiner Schließung direkt wieder benutzt werden kann.
  int flag = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

  // Der Socket wird an eine Addresse gebunden und der Server fängt an, auf Nachrichten zu warten, wobei maximal eine Nachricht gequeued werden kann.
  if((bind(s, (struct sockaddr *)&local, sizeof(local))) != 0){
    perror("Could not bind port");
    exit(1);
  }

  if((listen(s, 1)) != 0) {
    perror("Listening failed");
    exit(1);
  }

  // Die Liste der aktiven Ports wird initialisiert und der bereits eröffnete Socket wird hinzugefügt.
  FD_ZERO(&active_fds);
  FD_SET(s, &active_fds);

  // Der Server verfällt in eine Endlosschleife.
  while (1) {

    // Die Liste der potenziell lese- und schreibfähigen Ports wird aktualisiert.
    read_fds = active_fds;
    write_fds = active_fds;

    // Der Server blockiert, bis es auf einem Socket etwas zu lesen gibt.
    if (select(FD_SETSIZE, &read_fds, &write_fds, NULL, NULL) < 0) {
      perror("Could not select anything");
      exit(1);
    }

    // Alle möglichen Sockets werden durchgegangen.
    for (i = 0; i < FD_SETSIZE; i++) {

      // Es wird überprüft, ob der Socket aktiv ist.
      if (FD_ISSET(i, &read_fds)) {

        // Wenn es sich um den ursprünglichen Socket handelt, fordert offenbar ein neuer Client eine Verbindung an.
        if (i == s) {

          // Die Verbindung wird, wenn möglich, akzeptiert.
          len = sizeof(struct sockaddr_in);
          s2 = accept(s, (struct sockaddr *) &remote, (socklen_t *)&len);

          if (s2 < 0) {
            perror("Could not accept a connection");
            exit(1);
          }

          printf("Connection accepted!\n");

          // Der neue Socket wird den aktiven Sockets hinzugefügt.
          FD_SET(s2, &active_fds);

        } else { // Es handelt sich nicht um den ursprünglichen Socket, also versucht ein bereits bekannter und verbundener Client, mit dem Server zu kommunizieren.
          int read_bytes;

          memset(buf, '\0', sizeof buf);

          // Die Nachricht wird ausgelesen.
          read_bytes = recv(i, &buf, 512, 0);

          if (read_bytes < 0) {
            // Ein Fehler ist beim Lesen aufgetreten.
            perror("Could not read");
            exit(2);
          } else if (read_bytes == 0) {
            // Es wurde keine Nachricht bzw. ein EOF übergeben.
            close(i);
            FD_CLR(i, &active_fds);
          } else {
            // Falls QUIT gesendet wurde, wird die Verbindung zum Client getrennt.
            if (strstr(buf, "QUIT") != NULL) {
              printf("Recieved QUIT on socket %d. Closing connection...\n", i);
              close(i);
              FD_CLR(i, &active_fds);
            } else {
              // Andernfalls wird versucht, die Datei zu öffnen und den Inhalt an den Client zu schicken.
              memset(filename, '\0', sizeof filename);

              // Der Name der angeforderten Datei wird aus dem Request-Header gelesen, indem alle Zeichen nach "GET /" bis zum nächsten Leerzeichen in einen Puffer geschrieben werden.
              for (int i = 5; i < sizeof buf; i++) {
                if (buf[i] == ' ') {
                  break;
                }

                filename[i-5] = buf[i];
              }

              // Die Datei wird mit Leserechten geöffnet.
              FILE *in;
              in = fopen(filename, "r");

              // Es wird überprüft, ob die Datei geöffnet werden konnte.
              // Wenn nicht, wird stattdessen die 404-Seite geöffnet. 
              if (!in) {
                status = 404;
                printf("404 Could not get file %s\n\n", filename);
                strcpy(filename, "404.html");
                in = fopen(filename, "r");
              } else {
                status = 200;
                printf("200 Got file %s\n\n", filename);
              }

              char type[32];
              char extension[16];

              // Die letzten 4 Zeichen des Dateinamens (also in der Regel die Endung) werden in einen Puffer geschrieben.
              strcpy(extension, &filename[strlen(filename)-4]);

              // Basierend auf der Dateiendung wird der Typ der Datei ermittelt.
              if (strcmp(extension, "html") == 0) {
                strcpy(type, "text/html");
              } else if (strcmp(extension, "jpeg") == 0 || strcmp(extension, ".jpg") == 0) {
                strcpy(type, "image/jpeg");
              } else if (strcmp(extension, ".gif") == 0) {
                strcpy(type, "image/gif");
              } else {
                strcpy(type, "application/octet-stream");
              }

              struct stat filestats;
              stat(filename, &filestats);
              int size = filestats.st_size;

              // Es wird ein Puffer für die Datei alloziert.
              char *filebuf;
              filebuf = (char*)calloc(size, sizeof(char));

              // Die Datei wird ausgelesen und der Kanal geschlossen.
              fread(filebuf, sizeof(char), size, in);
              fclose(in);

              char header[1000000];

              // Der Header wird ohne die Datei konstruiert.
              int headersize = sprintf(header, "HTTP/1.0 %d\r\nContent-Type: %s\r\nConnection: close\r\nContent-Length: %d\r\n\r\n", status, type, size);

              // Header, Datei und Endung werden gesendet (dies geschieht in 3 Aufrufen, da send bei einem Aufruf nicht alles sendet).
              send(i, header, headersize, 0);

              send(i, filebuf, size, 0);

              send(i, "\r\n", sizeof("\r\n"), 0);

              // Der allozierte Puffer wird befreit und die Verbindung getrennt.
              free(filebuf);
              close(i);
              FD_CLR(i, &active_fds);
            }
          }
        }
      }
    }

  }
  return 0;
}
