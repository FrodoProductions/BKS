#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

// TODO: Socket als Argument!

int main() {
  unsigned int s, s2;
  struct sockaddr_un local, remote;
  int len;
  char buf[100];

  s = socket(AF_UNIX, SOCK_STREAM, 0);

  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, "csocket");
  unlink(local.sun_path);

  len = strlen(local.sun_path) + sizeof(local.sun_family);
  bind(s, (struct sockaddr *)&local, len);

  listen(s, 1);

  len = sizeof(struct sockaddr_un);
  s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&len);

  while (len = recv(s2, &buf, 100, 0), len > 0) {
    if (strcmp(buf, "QUIT") == 1) {
      printf("Recieved QUIT. Closing connection...\n");
      close(s2);
    }
    send(s2, "OK", len, 0);
  }

  return 0;

}
