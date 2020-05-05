#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[]) {

  char puffer[100];

  // Diese Art der Verwendung von access() hab ich in diesem StackOverflow-Thread gefunden:
  // https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
  if (access(*++argv, F_OK) == 0) {

    int in = open(*argv, O_RDONLY);
    int inputsize = read(in, &puffer, 1024);
    write(1, puffer, inputsize);
    close(in);

  } else {
    write(1, *argv, sizeof(*argv));
    write(1, "\n", 1);
  }

  return 0;
}
