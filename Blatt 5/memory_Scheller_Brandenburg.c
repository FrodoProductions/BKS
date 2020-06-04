#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

char mem[MEM_SIZE];

// Bemerkung: Wir verwenden hier eine doppeltverkettete Liste, da dies einige Vorgänge beim Freigeben von Speicherplatz erleichtert.
struct block {
  struct block *next;
  struct block *prev;

  size_t size;
  _Bool free;
};

// Head und dummy werden wie im supplement beschrieben initialisiert.
struct block *head = (struct block*) (&mem[0]);
struct block *dummy = (struct block*) (&mem[MEM_SIZE-sizeof(struct block)]);

void memory_init() {
  head -> next = dummy;
  head -> prev = dummy;

  // Head bekommt allen verfügbaren Speicherplatz zugeordnet und es wird markiert, dass dieser frei ist.
  head -> size = MEM_SIZE-sizeof(struct block)-sizeof(struct block);
  head -> free = 1;

  dummy -> next = head;
  dummy -> prev = head;
}

void *memory_allocate(size_t byte_count) {

  // Es wird zunächst ein Index deklariert. Dieser wird später dazu verwendet, dwn zu allozierenden Platz innerhalb der Array einfacher zu berechnen.
  size_t index = 0;

  // Wir iterieren durch alle Blöcke durch.
  for (struct block *i = head; i->next!=head; i=i->next) {

    // Es wird überprüft, ob der aktuelle Speicherbereich frei ist und die zu speichernde Menge sowie den neuen Block aufnehmen könnte.
    if (i -> free == 1 && i -> size >= (byte_count+sizeof(struct block))) {

      // Ein neuer Block wird am Ende des für byte_count benötigten Bereiches erzeugt.
      struct block *new = (struct block*) (&mem[index+sizeof(struct block)+byte_count]);

      // Der neue Block wird in die Kette integriert.
      new -> next = (i -> next);
      new -> prev = i;
      i -> next = new;
      new -> next -> prev = new;

      // Der neue Block bekommt nun den nach der Allokation noch freien Speicherbereich zugeordnet.
      new -> size = (i -> size)-byte_count-sizeof(struct block);
      new -> free = 1;

      // Der vorige Block verwaltet nun den neu allozierten Speicherbereich.
      i -> size = byte_count;
      i -> free = 0;

      // Hier bin ich mir nicht ganz sicher, ob der neue Block oder ein Verweis auf den Anfang des neu
      // allozierten Speicherbereiches zurückgegeben werden soll. Letzteres könne denke ich mit return(&index); bewerkstelligt werden.
      return(new);
    }

    // Der Index wird zum Anfang des nächsten Blockes verschoben.
    index += (sizeof(struct block)+i -> size);
  }

  // Die gesamte Kette wurde durchiteriert und kein freier Speicherplatz gefunden.
  return NULL;
}

void memory_free(const void *const pointer) {

  // pointer wird zu (struct block*) gecastet.
  struct block *stPointer = (struct block*) pointer;

  // Wenn der Bereich bereits frei ist, muss nichts gemacht werden.
  if(stPointer->free==1) {
    printf("Already free!\n");
    return;
  }

  // Solange sich "vor" und "hinter" dem freizumachenden Bereich andere freie Bereiche befinden,
  // werden diese zusammengelegt um Fragmentierung zu vermeiden.
  while (stPointer -> prev -> free == 1 || stPointer -> next -> free == 1) {

    // Fall: Vor dem Block steht bereits ein freier Block.
    if (stPointer -> prev -> free == 1) {

      // Ein Pointer auf den anderen freien Block wird gespeichert.
      struct block *freeBlock = stPointer -> prev;

      // Der insgesamt von dem freizumachenden Bereich belegte Speicher wird errechnet.
      size_t memoryToAdd = sizeof(struct block) + stPointer->size;

      // Der von dem Block allozierte Speicherplatz wird freigemacht.
      stPointer -> size = 0;

      // Der Block wird aus der Kette entfernt.
      stPointer -> next -> prev = freeBlock;
      freeBlock -> next = stPointer -> next;

      stPointer -> next = NULL;
      stPointer -> prev = NULL;

      // Der ehemalig allozierte Speicher wird dem freien Block angehängt.
      freeBlock -> size += memoryToAdd;

      // Der stPointer-Pointer wird auf den freien Block ausgerichtet, damit spätere Zusammenführungen von diesem aus durchgeführt werden können.
      stPointer = freeBlock;

    }

    // Fall: Hinter dem Block steht bereits ein freier Block.
    // Dieser Block soll entfernt und dessen Speicher dem aktuellen Block angehängt werden.
    if (stPointer -> next -> free == 1) {

      // Ein Pointer auf den anderen freien Block wird gespeichert.
      struct block *deleteBlock = stPointer -> next;

      // Der insgesamt von dem freizumachenden Bereich belegte Speicher wird errechnet.
      size_t memoryToAdd = sizeof(struct block) + deleteBlock -> size;

      // Der Block wird aus der Kette entfernt.
      deleteBlock -> next -> prev = stPointer;
      stPointer -> next = deleteBlock -> next;

      deleteBlock -> next = NULL;
      deleteBlock -> prev = NULL;

      // Der von dem Block allozierte Speicherplatz wird freigemacht.
      deleteBlock -> size = 0;

      // Der ehemalig freie Speicher wird dem aktuellen Block angehängt.
      stPointer -> size += memoryToAdd;

    }
  }

  // Der aktuelle Puffer wird als frei gekennzeichnet.
  stPointer -> free = 1;

  printf("Freed %d B.\n", stPointer -> size);
}

void memory_print() {

  // Ein Index wird deklariert, um den Blöcken eine Nummer zuordnen zu können.
  size_t index = 0;

  printf("Block#\tSize\tFree?\n\n");

  // Es wird durch die Kette iteriert.
  for (struct block *i = head; i->next!=head; i=i->next) {
    // Es werden der aktuelle Index, die Größe des zugehörigen Speicherbereiches in Byte und der Status des Blocken ausgegeben.
    printf("%d\t%d B\t%d\n", index, (i->size), i->free);
    index++;
  }

  // Die newline ist eigentlich nur dazu da, damit es etwas besser aussieht :>
  printf("\n");
}

void *memory_by_index(size_t index) {

  // In dieser Funktion werden die verschiednenen Blöcke durchgezählt, bis der gewünscht Index erreicht ist.

  size_t count = 0;

  for (struct block *i = head; i->next!=head; i=i->next) {
    if (count == index) {
      return i;
    }
    count++;
  }

  return NULL;
}
