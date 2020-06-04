/* (c) 2018 Leonard König -- leonard.koenig@fu-berlin.de */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "memory.h"

int main(void)
{
	printf("Initializing Memory\n");
	memory_init();
	printf("Usage:\n"
	       " a <bytes>\tallocate memory\n"
	       " f <index>\tfree block at index\n"
	       " p        \tprint block list\n"
	       " q        \tquit\n");

	bool quit = false;
	while (!quit) {
		char *const input = readline("> ");
		if (!input) { free(input); break; }

		char cmd;
		int len;
		if (sscanf(input, " %c%n", &cmd, &len) != 1) {
			fprintf(stderr, "Command expected!\n");
			free(input);
			continue;
		}

		unsigned long bytes;
		unsigned index;
		switch (cmd) {
		case 'a':
			if (sscanf(&input[len], " %lu", &bytes) != 1) {
				fprintf(stderr, "allocate expects an unsigned number\n");
        			free(input);
				continue;
			}
			if (!memory_allocate(bytes)) {
				fprintf(stderr, "Couldn't allocate %lu bytes\n", bytes);
        			free(input);
				continue;
			}
			printf("Allocated %lu bytes\n", bytes);
			break;
		case 'f':
			if (sscanf(&input[len], " %u", &index) != 1) {
				fprintf(stderr, "free expects an unsigned index\n");
        			free(input);
				continue;
			}
			const void *const p = memory_by_index(index);
			if (!p) {
				fprintf(stderr, "Memory block %u doesn't exist\n", index);
        			free(input);
				continue;
			}
			memory_free(p);
			break;
		case 'p':
			memory_print();
			break;
		case 'q':
			quit = true;
			break;
		default:
			fprintf(stderr, "Unknown command '%c'\n", cmd);
			break;
		}

		free(input);
	}

	return 0;
}
