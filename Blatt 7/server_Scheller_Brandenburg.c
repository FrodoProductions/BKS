#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char const *argv[]) {
  unsigned int s, s2;
  struct sockaddr_un local, remote;
  int len;
  char buf[100];

  // Es wird überprüft, ob ein Pfad angegeben wurde.
  if (argc < 2) {
    perror("Please provide a path.");
    exit(1);
  } else if (argc > 2) {
    perror("Unexpected arguments. Please provide only one path.");
    exit(1);
  }

  // Ein Stream-Socket wird geöffnet.
  s = socket(AF_UNIX, SOCK_STREAM, 0);

  // Der Pfad wird aus den angegeben Argumenten erzeugt.
  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, argv[1]);

  // Falls sich bereits ein Socket an dem angegebenen Pfad befindet, wird dieser aufgelöst.
  unlink(local.sun_path);

  // Der Socket wird an den Pfad gebunden und der Server fängt an, auf Nachrichten zu warten, wobei maximal eine Nachricht gequeued werden kann
  len = strlen(local.sun_path) + sizeof(local.sun_family);
  bind(s, (struct sockaddr *)&local, len);

  listen(s, 1);

  // Nun wird die bilaterale Verbindung mithilfe von accept() hergestellt. Hierbei wird ein neuer Socket eröffnet, über den die Nachrichten später empfangen werden.
  len = sizeof(struct sockaddr_un);
  s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&len);

  // Der Server schaut nun so lange nach Nachrichten des Clients ausschau, bis dieser das QUIT-Signal schickt.
  while (len = recv(s2, &buf, 100, 0), len > 0) {
    // Erhält der Server das QUIT-Signal vom Client, so wird der Socket s2 geschlossen.
    if (strcmp(buf, "QUIT") == 1) {
      printf("Recieved QUIT. Closing connection...\n");
      close(s2);
    }
    // Andernfalls schickt der Server das OK-Signal an den Client zurück, welches dieser dann ausgibt.
    send(s2, "OK", len, 0);
  }

  return 0;

}
