//Authors: Scheller, Brandenburg
#include <stdio.h>
#include <ctype.h>

// Diese Funktion übernimmt, wenn sie die entsprechenden Argumente bekommt, die eigentliche Berechnung.
int calculate(int operand1, int operand2, char operator) {

	// Die Art der durchzuführenden Operation wird anhand des Operators ermittelt und das Ergebnis in operand1 geschrieben.
	switch(operator) {
		case '+': operand1 = operand1 + operand2; break;
		case '-': operand1 = operand1 - operand2; break;
		case '*': operand1 = operand1 * operand2; break;
		case '/': operand1 = operand1 / operand2; break;
		default: printf("Unbekannter Operand. bc unterstützt nur +, -, * und /.\n"); break;

	}

	// Das Ergebnis wird zurückgegeben.
	return operand1;
}

int main(int argc, char const *argv[]) {

	// Es wird geguckt, ob beim Augruf Argumente (Dateien) übergeben wurden.
	if (argc > 1) {

		// Es wird durch jede Datei iteriert.
		for (size_t i = 1; i < argc; i++) {

			// Die aktuelle Datei wird mit Leserechten geöffnet.
			FILE* datei = fopen(argv[i], "r");

			// Es wird überprüft, ob die Datei existiert. Falls nicht, wird mit Code 1 beendet.
			if (datei == NULL) {
				printf("%s\n", "File not found.");
				return 1;
			}

			int operand1, operand2;
			char operator;

			// Die Datei wird nach dem Pattern "%d %c %d\n" (also Zahl, Operator, Zahl, Newline) durchsucht.
			// Jedes dieser Patterns wird gespeichert und der calculate() Funktion übergeben.
			while (fscanf(datei, "%d %c %d\n", &operand1, &operator, &operand2) == 3) {
				printf("%d\n", calculate(operand1, operand2, operator));
			}
		}

	} else {

		// Da keine Dateien übergeben wurden, startet der interaktive Modus.
		printf("CALCULATOR\n");

		int operand1, operand2, exit;
		char operator;

		// Der interaktive Modus startet in einer Endlosschleife.
		while (1) {

			// Die Standardeingabe wird nach dem Pattern "%d %c %d" (also Zahl, Operator, Zahl) durchsucht.
			scanf("%d %c %d", &operand1, &operator, &operand2);

			// Es wird überprüft, ob irgendein Argument nicht gefunden wurde.
			if (isblank(operand1) || isblank(operand2) || isblank(operator)){
				printf("Invalid Inputs, try something like 1 + 1\n");
			} else {
				// Alle Argumente wurden gefunden und werden der calculate() Funktion übergeben.
				printf("%d\n", calculate(operand1, operand2, operator));

			}
		}
	}
	return 0;
}
