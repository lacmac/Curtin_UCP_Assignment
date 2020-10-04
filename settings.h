#ifndef SETTINGS_H
#define SETTINGS_H

#ifndef EFFECTS_H
#define EFFECTS_H
#include "effects.h"
#endif

#define MIN_COL_CODE 0
#define MAX_FG_CODE 15
#define MAX_BG_CODE 7
#define WHITE_FG 15
#define WHITE_BG 7
#define BLACK 0

/**
 * A struct which keeps track of the current TurtleGraphics options
 */
typedef struct
{
    struct
    {
        double x;
        double y;
    } pos;
    double angle;
    int fgColour;
    int bgColour;
    char pattern;
} TurtleSettings;

TurtleSettings* createSettings();

void getPos(TurtleSettings* settings, double* x, double* y);

void resetColours();

void setColoursSimple();

#endif
