#ifndef FILEIO_H
#define FILEIO_H

#include "boolean.h"
#include "command.h"
#include "linkedList.h"
#include "utils.h"

int validateInputFile(char* fileName);

int validateLine(char line[], int* isEmpty);

LinkedList* readCommandsFromFile(char* fileName);

void processLine(LinkedList* cmdList, char* line);

#endif
