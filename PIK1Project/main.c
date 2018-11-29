#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

/*
 * Main file for the project
 */

enum MenuOptions
{
	ReadFromFileWriteToFile = 0,
	ReadFromFileWriteToSTDOUT = 1,
	ReadFromSTDINWriteToFile = 2,
	ReadFromSTDINWriteToSTDOUT = 3
};

int menuPicker();
void validateSelectedOperation(int * selectedOperation);
void printMenu(int selectedOption, char activeOperation);

int main()
{
	SetConsoleOutputCP(1251);

	// pick function for the selected operation
	switch (menuPicker())
	{
	case ReadFromFileWriteToFile: break;
	case ReadFromFileWriteToSTDOUT: break;
	case ReadFromSTDINWriteToFile: break;
	case ReadFromSTDINWriteToSTDOUT: break;
	default:
		break;
	}
	system("pause");
	return 0;
}

int menuPicker()
{
	int selectedOperation = 0;
	char activeOperation = '>', enterKey = 13, userClick, upwardKey = 80, downwordKey = 72;
	printMenu(selectedOperation, activeOperation);
	while (getch() != enterKey) {
		// as I want to use the arrow keys we need to take the second char too as it differentiates them
		userClick = getch();
		if (userClick == upwardKey) {
			selectedOperation++;
		}
		else if (userClick == downwordKey) {
			selectedOperation--;
		}
		validateSelectedOperation(&selectedOperation);
		printMenu(selectedOperation, activeOperation);
	}
	return selectedOperation;
}

void validateSelectedOperation(int * selectedOperation)
{
	if (*selectedOperation < 0) {
		*selectedOperation = 3;
	}
	if (*selectedOperation >= 4) {
		*selectedOperation = 0;
	}
}

void printMenu(int selectedOption, char activeOperation)
{
	system("cls");
	char * opertaions[] = { "четене от фаил и запис в фруг фаил",
						 "четене от фаил и извеждане на stdout",
						 "четене от stdin и извеждане в фаил",
						 "четене от stdin и извждане от stdout" };

	for (int i = 0; i < 4; i++) {
		if (i == selectedOption) {
			printf("%c", activeOperation);
		}
		else {
			printf(" ");
		}

		printf("%s\n", opertaions[i]);
	}
}
