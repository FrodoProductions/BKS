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

	char puffer[100];

	if (argc > 1) {

		//insert interesting stuff here

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
