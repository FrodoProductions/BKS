#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int selectNonInvisible(const struct dirent *file) {
  return(strncmp(file->d_name, ".", 1));
}

int main(int argc, char const *argv[]) {

  struct dirent **files;
  size_t fileCount;

  int c;
  int (*filter) (const struct dirent*) = &selectNonInvisible;

  char *path = NULL;

  while((c = getopt (argc, argv, "al:")) != -1) {

    path = optarg;

    switch (c) {
      case 'a':
        filter = NULL;
        break;
      case 'l':
        // do something
      case ':':
        printf("ls needs a path!\nusage: ls [-al] [file ...]\n");
        break;
      case '?':
        printf("ls: illegal option -- %c\nusage: ls [-al] [file ...]\n", optopt);
        break;
    }
  }

  fileCount = scandir(argv[optind], &files, (*filter), alphasort);

  printf("%d files detected in this directory!\n", fileCount);

  for (size_t i = 0; i < fileCount; i++) {
    printf("%s\t", files[i]->d_name);
  }

  printf("\n");

  free(files);
  return 0;
}
