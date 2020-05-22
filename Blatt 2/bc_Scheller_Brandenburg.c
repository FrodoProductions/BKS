//Authors: Scheller, Brandenburg
#include <stdio.h>
#include <ctype.h>


int calculate(int operand1, int operand2, char operator) {

	switch(operator) {
		case '+': operand1 = operand1 + operand2; break;
		case '-': operand1 = operand1 - operand2; break;
		case '*': operand1 = operand1 * operand2; break;
		case '/': operand1 = operand1 / operand2; break;
	}

	return operand1;
}

int main(int argc, char const *argv[]) {

	//char puffer[100];

	if (argc > 1) {

		for (size_t i = 1; i < argc; i++) {
			FILE* datei = fopen(argv[i], "r");

			if (datei == NULL) {
				printf("%s\n", "File not found.");
				return 1;
			}

			char puffer[100];

			int operand1, operand2;
			char operator;

			/* Funktioniert, ist mir aber nicht elegant genug
			while (fscanf(datei, "%s\n", &puffer)==1) {
				sscanf(puffer, "%d %c %d", &operand1, &operator, &operand2);
				printf("%d\n", calculate(operand1, operand2, operator));
			}
			*/

			while (fscanf(datei, "%d %c %d\n", &operand1, &operator, &operand2) == 3) {
				printf("%d\n", calculate(operand1, operand2, operator));
			}
		}

	} else {

		printf("CALCULATOR\n");
		int operand1, operand2, exit;
		char operator;
		while (1) {

			scanf("%d %c %d", &operand1, &operator, &operand2);
			if (isblank(operand1) || isblank(operand2) || isblank(operator)){
				printf("Invalid Inputs, try something like 1 + 1\n");
			} else {
				printf("%d\n", calculate(operand1, operand2, operator));

			}
		}
	}
	return 0;
}
