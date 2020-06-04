/* (c) 2018 Leonard König -- leonard.koenig@fu-berlin.de */

#ifndef _MM_H
#define _MM_H

#include <stddef.h>

#define MEM_SIZE 1024

void memory_init(void);
void *memory_allocate(size_t byte_count);
void memory_free(const void *const pointer);
void memory_print(void);

/* needed for wrapper */
void *memory_by_index(size_t index);

#endif
