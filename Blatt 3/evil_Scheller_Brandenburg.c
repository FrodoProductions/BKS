#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


void endlos() {

  struct timespec zeit;

  zeit.tv_sec = 2;

  printf("%d\n", getpid());

  while (1) {
    nanosleep(&zeit, NULL);
    printf("Endlosschleife\n");
  }

}

int main(int argc, char const *argv[]) {

  struct sigaction action;

  action.sa_handler = &endlos;

  sigaction(SIGINT, &action, NULL);

  while (1) {
    sleep(10);
  }

  return 0;
}
