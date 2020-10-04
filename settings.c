#include <stdlib.h>
#include "settings.h"

/**
 * Allocates enough memory for a TurtleSettings struct and initialises all fields
 * to their default values and returns the TurtleSettings struct.
 *
 * Returns:
 *  settings - a default TurtleSettings struct
 */
TurtleSettings* createSettings()
{
    TurtleSettings* settings = (TurtleSettings*) malloc(sizeof(TurtleSettings));

    settings->pos.x = 0;
    settings->pos.y = 0;
    settings->angle = 0.0;
    settings->fgColour = WHITE_FG;
    settings->bgColour = BLACK;
    settings->pattern = '+';

    return settings;
}

/**
 * Using pointers, exports the current 'x' and 'y' position of the TurtleSettings
 * struct.
 *
 * Parameters:
 *  settings - the TurtleSettings struct to retrieve the coords from
 *  x        - (export) the current x coordinate
 *  y        - (export) the current y cooridnate
 */
void getPos(TurtleSettings* settings, double* x, double* y)
{
    *x = settings->pos.x;
    *y = settings->pos.y;
}

/**
 * Reset the foreground and background colours of the terminal to normal.
 */
void resetColours()
{
    setFgColour(WHITE_FG);
    setBgColour(BLACK);
}

/**
 * Set the foreground and background colours of the terminal so it's black
 * characters on a white background.
 */
void setColoursSimple()
{
    setFgColour(BLACK);
    setBgColour(WHITE_BG);
}
