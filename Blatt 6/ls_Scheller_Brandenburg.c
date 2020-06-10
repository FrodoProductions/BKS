#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <langinfo.h>

int selectNonInvisible(const struct dirent *file) {
  return(strncmp(file->d_name, ".", 1));
}

int main(int argc, char const *argv[]) {

  struct dirent **files;
  struct stat buffer;

  size_t fileCount;

  char c;
  int printLong = 0;
  int (*filter) (const struct dirent*) = &selectNonInvisible;

  const char *path;

  while((c = getopt (argc, argv, ":al")) != -1) {

    switch (c) {
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

  if (argv[optind] == NULL) {
    path = ".";
  } else if (opendir(argv[optind])) {
    path = argv[optind];
  } else {
    printf("ls: directory either non-existant or access has not been granted.\n");
    return 2;
  }

  fileCount = scandir(path, &files, (*filter), alphasort);

  printf("%d files detected in this directory!\n", fileCount);

  for (size_t i = 0; i < fileCount; i++) {

    if (printLong) {
      char fullpath[512];
      strcpy(fullpath, path);
      strcat(fullpath, "/");
      strcat(fullpath, files[i]->d_name);

      stat(fullpath, &buffer);

      printf("%10s\t", strtok(ctime(&buffer.st_mtime), "\n"));
    }

    printf("%-10s\n", files[i]->d_name);
  }

  printf("\n");

  free(files);
  return 0;
}
