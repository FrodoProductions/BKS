#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {
  unsigned int s, s2;
  struct sockaddr_in local, remote;
  int len;
  char buf[100];

  // Es wird überprüft, ob ein Pfad angegeben wurde.
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

  // Der Pfad wird aus den angegeben Argumenten erzeugt.
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = inet_addr(argv[1]);
  local.sin_port = htons(atoi(argv[2]));

  // Der Socket wird als reusable eingestuft, damit er nach seiner Schließung direkt wieder benutzt werden kann.
  int flag = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

  // Der Socket wird an den Pfad gebunden und der Server fängt an, auf Nachrichten zu warten, wobei maximal eine Nachricht gequeued werden kann
  if((bind(s, (struct sockaddr *)&local, sizeof(local))) != 0){
    perror("Could not bind port");
    exit(1);
  }

  if((listen(s, 1)) != 0) {
    perror("Listening failed");
    exit(1);
  }

  // Nun wird die bilaterale Verbindung mithilfe von accept() hergestellt. Hierbei wird ein neuer Socket eröffnet, über den die Nachrichten später empfangen werden.
  len = sizeof(struct sockaddr_in);

  s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&len);

  if(s2 < 0) {
    perror("Server didn't accept connection");
    close(s2);
    exit(1);
  } else {
    printf("Connection accepted.\n");
  }

  // Der Server schaut nun so lange nach Nachrichten des Clients ausschau, bis dieser das QUIT-Signal schickt.
  while (len = recv(s2, &buf, 100, 0), len > 0) {
    // Erhält der Server das QUIT-Signal vom Client, so wird der Socket s2 geschlossen.
    if (strstr(buf, "QUIT") != NULL) {
      printf("Recieved QUIT. Closing connection...\n");
      close(s);
      close(s2);
    }
    // Andernfalls schickt der Server das OK-Signal an den Client zurück, welches dieser dann ausgibt.
    send(s2, "OK", len, 0);
  }

  return 0;

}

// QUELLE: http://beej.us/guide/bgipc/html/multi/unixsock.html (18.06.2020, 18:23 Uhr)
