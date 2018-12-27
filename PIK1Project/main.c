#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>

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
char * substr(char* str, char * startSubstr, char * endSubstr, int lengthOfSymbols);
void validateSelectedOperation(int * selectedOperation);
void printMenu(int selectedOption, char activeOperation);
void operateStreams(FILE * inStream, FILE * outStream);
int isLineEmpty(char * line);
char * removeStrings(char * line);
void removeInlineComments(char * line);
char * removeMultiLineComments(char * line, bool * isInMultilineComment);
int getNumberOfOperators(char * line, bool * isInMultiLineComment);
void * numberToString(int number, char * string);
void readUserInput(bool fromInputFile, bool fromOutputFile, FILE ** inputFile, FILE ** outputFile);

int main()
{
	SetConsoleOutputCP(1251);

	FILE ** inputFile = NULL, **outputFile = NULL;

	// pick function for the selected operation
	switch (menuPicker())
	{
	case ReadFromFileWriteToFile: readUserInput(true, true, &inputFile, &outputFile); break;
	case ReadFromFileWriteToSTDOUT: readUserInput(true, false, &inputFile, &outputFile); break;
	case ReadFromSTDINWriteToFile: readUserInput(false, true, &inputFile, &outputFile); break;
	case ReadFromSTDINWriteToSTDOUT: readUserInput(false, false, &inputFile, &outputFile); break;
	default:
		break;
	}
	operateStreams(inputFile, outputFile);
	char sad[] = "beforeString\"\"afterString";
	removeStrings(sad);
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

void operateStreams(FILE * inStream, FILE * outStream)
{
	char lineBuffer[128], lastLineChar = '\n';
	memset(lineBuffer, 0, 128 * (sizeof lineBuffer[0]));
	int numberOfEmptyLines = 0, numberOfOperators = 0;
	bool isInMultiLineComment = false;
	while (fgets(lineBuffer, sizeof(lineBuffer), inStream)) {
		lastLineChar = lineBuffer[strlen(lineBuffer) - 1];
		numberOfEmptyLines += isLineEmpty(lineBuffer);
		numberOfOperators += getNumberOfOperators(lineBuffer, &isInMultiLineComment);
		memset(lineBuffer, 0, 128 * (sizeof lineBuffer[0]));
	}

	numberOfEmptyLines = lastLineChar == '\n' ? numberOfEmptyLines + 1 : numberOfEmptyLines;

	char numAsString[100] = {'0', '\0'};
	fputs("Брой празни редове: ", outStream);
	fputs(numberToString(numberOfEmptyLines, numAsString), outStream);
	fputs(" Брой оператори: ", outStream);
	fputs(numberToString(numberOfOperators, numAsString), outStream);
	fputc('\n', outStream);
}

int isLineEmpty(char *line) {
	for (int i = 0; i < strlen(line); i++) {
		if (!isspace(line[i])) {
			return false;
		}
	}
	return true;
}

int getNumberOfOperators(char * line, bool * isInMultiLineComment) {
	int matches = 0;
	char * comment, *operator;
	char *operators[] = { "while", "break", "if", "continue", "switch", "case"}; // TODO add them all
	line = removeMultiLineComments(line, isInMultiLineComment);
	line = removeStrings(line);
	removeInlineComments(line);
	for (int i = 0; i < 6; i++) {
		int k = 0;
		operator = line;
		while ((operator = strstr(operator + k, operators[i])) != NULL) {
			k += strlen(operators[i]);
			matches++;
		}
	}
	return matches;
}

void readUserInput(bool fromInputFile, bool fromOutputFile, FILE ** inputFile, FILE ** outputFile)
{
	char inputFileName[40], outputFileName[40];
	if (fromInputFile) {
		printf("Моля въведете името на фаила за четене: ");
		scanf("%s", inputFileName);
		*inputFile = fopen(inputFileName, "r+");
	}
	else {
		*inputFile = stdin;
	}

	if (fromOutputFile) {
		printf("Моля въведете името на фаила за писане на резултат: ");
		scanf("%s", outputFileName);
		*outputFile = fopen(outputFileName, "w+");
	}
	else {
		*outputFile = stdout;
	}
}


char * numberToString(int number, char * string) {
	if (number == 0) {
		return string;
	}

	int i = 0;
	while (number > 0) {
		char digit = '0' + number % 10;
		number /= 10;
		string[i] = digit;
		i++;
	}
	
	string[i] = '\0';

	for (int k = 0; k < strlen(string) / 2; k++) {
		char swap = string[k];
		string[k] = string[strlen(string) - 1 - k];
		string[strlen(string) - 1 - k] = swap;
	}
	return string;
}

char *  removeStrings(char * line) {
	char * start = NULL;
	char * end = NULL;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == '\"') {
			if (start == NULL) {
				start = line + i;
			}
			else {
				end = line + i;
			}
		}
	}
	if (start != NULL && end != NULL) {
		line = substr(line, start, end, 1);
	}
	return line;
}

void removeInlineComments(char * line) {
	char * comment = strstr(line, "//");
	if (comment != NULL) {
		*comment = '\0';
	}
}

char * removeMultiLineComments(char * line, bool * isInMultilineComment) {
	char * startComment = strstr(line, "/*");
	char * endComment = strstr(line, "*/");
	if (*isInMultilineComment) {
		if (endComment != NULL) {
			line = endComment + 2; // plus 2 as */ and we want to pass this
			*isInMultilineComment = false;
		}
		else {
			*line = '\0';
		}
	}
	else {
		if (startComment != NULL && endComment != NULL) {
			line = substr(line, startComment, endComment, 2);
		}
		else {
			if (startComment != NULL) {
				*isInMultilineComment = true;
				*startComment = '\0';
			}
		}
	}

	return line;
}

char * substr(char* str, char * startSubstr, char * endSubstr, int lengthOfSymbols) {
	char returnStr[256] = { '\0' };
	*startSubstr = '\0';
	strcat(returnStr, str);
	strcat(returnStr, endSubstr + lengthOfSymbols);

	return returnStr;
}