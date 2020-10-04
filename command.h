#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include "settings.h"
#ifndef EFFECTS_H
#define EFFECTS_H
#include "effects.h"
#endif
#include "linkedList.h"
#include "utils.h"

#define MAX_CMD_NAME_SIZE 7
#define MAX_CMD_PARAM_SIZE 10

/**
 * A struct to store a Command. The struct contains the name (char array) of the
 * command and a void pointer to the value of the command. A void pointer is used
 * as the value could either be a double, int, or char.
 */
typedef struct
{
    struct
    {
        char value[MAX_CMD_NAME_SIZE];
        size_t length;
    } name;
    void* value;
} Command;

Command* createCommand(char name[], size_t len, void* value);

void executeCommand(TurtleSettings* settings, Command* command, FILE* logFile);

void rotate(TurtleSettings* settings, double angle);

void move(TurtleSettings* settings, double distance, double* deltaX, double* deltaY);

void draw(TurtleSettings* settings, double distance, double* deltaX, double* deltaY);

int isCommandWithRealArg(char cmdName[]);

int isCommandWithIntArg(char cmdName[]);

int isCommandWithCharArg(char cmdName[]);

int isOutOfBounds(char cmdName[], char cmdValue[]);

double adjustAngle(double angle);

void adjustDeltas(double* deltaX, double* deltaY);

void printPattern(void* pattern);

void freeCommand(void* command);

#endif
