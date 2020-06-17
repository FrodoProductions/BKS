#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

// TODO: Socket als Argument!

int main() {
  unsigned int s;
  struct sockaddr_un remote;
  int len, t;
  char buf[100];

  s = socket(AF_UNIX, SOCK_STREAM, 0);

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, "csocket");
  len = strlen(remote.sun_path) + sizeof(remote.sun_family);

  if(connect(s, (struct sockaddr *)&remote, len) == -1) {
    perror("connect");
    exit(1);
  }

  printf("Connected.\n");

  while (printf("> "), fgets(buf, 100, stdin), !feof(stdin)) {
    if (send(s, buf, strlen(buf),0) == -1) {
      perror("send");
      exit(1);
    }

    if ((t=recv(s, buf, 100, 0)) > 0) {
      buf[t] = '\0';
      printf("server> %s\n", buf);
    } else {
      if (t < 0) perror("recv");
      else printf("Server closed connection\n");
      exit(1);
    }
  }

  return 0;
}
