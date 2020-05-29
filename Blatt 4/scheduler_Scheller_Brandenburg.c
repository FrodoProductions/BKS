#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

/* Round Robin */
void rr(struct process *head){
//Wir haben als Interval: (RR),q = 1
  //Hilfs construct current initialisieren, damit wir alle Prozesse untersuchen können
  struct process *current = head->next;
  //Current wird auf den Prozess gesetzt, der im letzten tick ausgeführt wurde.
  while (current != head && current->state != PS_RUNNING){
    current = current->next;
  }
  //Je nachdem, ob current noch weiter ausgeführt werden muss oder nicht, wird current entweder auf PS_READY oder PS_DEAD gesetzt.
  if (current->cycles_todo > 0){
    current->state = PS_READY;
  } else {
    current->state = PS_DEAD;
  }
  //Nun werden alle Prozesse durchgegangen (von Current ausgegangen immer jeweils der Vorgänger). Der erste Vorgänger, der
  //entweder noch Ausgeführt werden muss oder wieder Current ist, wird dann ausgewählt.
  current = current->prev;
  struct process *help = current->next;
  while (current->cycles_todo == 0 && current != help){
    current = current->prev;
  }
  //Falls Current noch ausgeführt werden kann, so wird der Prozess ausgeführt.
  if (current->state == PS_READY){
  current->state = PS_RUNNING;
  }
}

/* First Come First Serve */
void fcfs(struct process *head){
  //Hilfs construct current initialisieren, damit wir alle Prozesse untersuchen können
  struct process *current = head->next;
    //Schleife zum durchlaufen aller Przesse: wenn Current fertig ausgeführt ist und nicht Head ist,
    //so wird der nächste Prozess zu current. Wenn Current fertig ausgeführt ist wird der Prozess in PS_DEAD versetzt.
    while (current->cycles_todo == 0 && current != head) {
      current->state = PS_DEAD;
      current = current-> next;
    }
    //Nun muss Current noch in den PS_RUNNING Zustand gebracht werden. Dies soll nur geschehen, wenn Current nicht Head ist.
    if (current != head){
      current->state = PS_RUNNING;
    }
}

/* Shortest Process Next */
void spn(struct process *head){
//Zwei Hilfs cunstructs werden initialisiert.
struct process *current = head->next;
struct process *next = head-> next;
//Überprüfen, ob es einen Laufenden Prozess gibt
while (current != head && current->state != PS_RUNNING){
  current = current->next;
}
//Für next wird ein Prozess gesucht, der mindestens noch für einen Cycle ausgeführt werden muss
while (next != head && next->cycles_todo == 0){
  next = next->next;
}
//Falls es einen Prozess gibt, der ausgeführt werden soll und auch noch ausgeführt werden muss, soll nichts getan werden.
if (current->cycles_todo > 0){
  //*Nichts*
} else {
  //Nun werden alle Prozesse durchgegangen, solche, die keine Cycles mehr brauchen, werden in den PS_DEAD Zustand versetzt,
  //alle anderen in PS_READY.
  current = head->next;
  while (current != head){
    if(current->cycles_todo > 0){
      current->state = PS_READY;
      //Hier wird die Länge des aktuell zu untersuchenden Prozesses mit dem bis jetzt am kürzesten verglichen und falls kürzer
      //wird next auf current gesetzt.
      if((current->cycles_todo + current->cycles_done) < (next->cycles_todo + next->cycles_done)){
        next = current;
      }
    } else {
      current->state = PS_DEAD;
    }
      current = current->next;
    }
    //Wenn nun der resultierende Prozess nicht head ist, soll dieser in PS_RUNNING versetzt werden.
    if (next != head){
      next->state = PS_RUNNING;
    }
  }
}
/* Shortest Remaining Time */
void srt(struct process *head){
    //Zwei Hilfs cunstructs werden initialisiert.
    struct process *current = head->next;
    struct process *next = head-> next;
    //Für next wird ein Prozess gesucht, der mindestens noch für einen Cycle ausgeführt werden muss
    while (next != head && next->cycles_todo == 0){
      next = next->next;
    }
    //nun werden alle Prozesse durchgegangen, solche, die keine Cycles mehr brauchen, werden in den PS_DEAD Zustand versetzt,
    //alle anderen in PS_READY.
    while (current != head){
      if (current->cycles_todo > 0){
        current->state = PS_READY;
        //Hier werden die noch auszuführenden cycles des aktuell zu untersuchenden Prozesses und des aktuell kürzesten verglichen
        //und falls größer wird next auf current gesetzt.
        if(current->cycles_todo < next->cycles_todo){
          next = current;
        }
      } else {
        current->state = PS_DEAD;
      }
    current = current->next;
    }
    //Wenn nun der resultierende Prozess nicht head ist, soll dieser in PS_RUNNING versetzt werden.
    if (next != head){
      next->state = PS_RUNNING;
    }
}


/* Highest Response Ration Next */
void hrrn(struct process *head){
  //Zwei Hilfs cunstructs werden initialisiert.
  struct process *current = head->next;
  struct process *next = head-> next;
  //Überprüfen, ob es einen Laufenden Prozess gibt
  while (current != head && current->state != PS_RUNNING){
    current = current->next;
  }
  //FÜr next wird ein Prozess gesucht, der noch nicht abgeschlossen ist.
  while (next != head && next->cycles_todo == 0){
    next = next->next;
  }

  //Falls es einen Prozess gibt, der ausgeführt werden soll und auch noch ausgeführt werden muss, soll nichts getan werden.
  if (current->cycles_todo > 0){
    //*Nichts*
  } else {
    //Alle Prozesse werden gesucht und derjenige mit dem Highest Response Ratio wird ausgewählt, Prozesse die
    //noch ausgeführt werden müssen werden in PS_READY versetzt und schon ausgeführte werden in PS_DEAD versetzt.
    current = head->next;
    while (current != head){
        if (current->cycles_todo > 0){
          current->state = PS_READY;
            if ((current->cycles_waited + current->cycles_todo) / current->cycles_todo > (next->cycles_waited + next->cycles_todo) / next->cycles_todo){
              next = current;
            }
        } else {
          current->state = PS_DEAD;
        }
    current = current->next;
    }
  }
  //falls der Prozess nun nicht gleich head ist, so wird dieser in PS_RUNNING versetzt.
  if (next != head){
    next->state = PS_RUNNING;
  }
}
