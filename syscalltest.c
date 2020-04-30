#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[]) {

  char puffer[100];

  int inch = open(*++argv, O_RDONLY);

  int inputsize = read(inch, puffer, 1024);
  write(1, puffer, inputsize);

  close(inch);

  return 0;
}
