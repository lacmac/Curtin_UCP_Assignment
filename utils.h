#ifndef UTILS_H
#define UTILS_H

#include "boolean.h"

/* Define PI as math.h does not include it in C89 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void convertToUpperCase(char string[]);

int isReal(char cmdValue[]);

int isInteger(char cmdValue[]);

int isCharacter(char cmdValue[]);

void polToRec(double d, double angle, double* x, double* y);

double roundNum(double n);

void removeNewLineChar(char* str);

#endif