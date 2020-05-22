#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[]) {

  //ÜBerprüfen, ob der erste Input eine Signalnummer ist (Schamlos wird davon ausgegangen, das wir hier nicht eine PID unter 50 haben)
  if (atoi(argv[1]) < 50){
    //Für jede eingegebene PID wird der kill Befehl aufgerufen
    for (int i = 2; i < argc; i++){
      kill(atoi(argv[i]), atoi(argv[1]));
    }
  }
  //Analog wie oben, jedoch mit festen SIGTERM
  else {
    for (int i = 1; i < argc; i++){
      kill(atoi(argv[i]), SIGTERM);
    }
  }
}
