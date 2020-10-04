#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define LOG_FORMAT "%s (%7.3f, %7.3f)-(%7.3f, %7.3f)\n"

/**
 * Allocates the appropriate amount of memory for a command. The memory allocated
 * depends on the name of the command, as the void pointer can take on either a
 * double, int, or char.
 *
 * Parmaeters:
 *  name  - the name of the command in uppercase, e.g. MOVE
 *  len   - the length of the name characterv array
 *  value - the value of the command ,e.g. 5.0
 * Returns:
 *  command - the Command struct created and malloc'ed from the imports
 */
Command* createCommand(char name[], size_t len, void* value)
{
    Command* command = (Command*) malloc(sizeof(Command));

    if (isCommandWithRealArg(name))
    {
        command->value = malloc(sizeof(double));
        memcpy(command->value, value, sizeof(double));
    }
    else if (isCommandWithIntArg(name))
    {
        command->value = malloc(sizeof(int));
        memcpy(command->value, value, sizeof(int));
    }
    else if (isCommandWithCharArg(name))
    {
        command->value = malloc(sizeof(char));
        memcpy(command->value, value, sizeof(char));
    }

    strncpy(command->name.value, name, len);
    command->name.length = len;

    return command;
}

/**
 * Executes a command located in the Command struct and updates the settings struct
 * with the result of the command. The log file is printed to when a DRAW or MOVE
 * command is executed. The log file prints the coordinates before and after the
 * move or draw.
 *
 * Parameters:
 *  settings - the TurtleSettings struct which holds the current options
 *  command  - the Command struct to execute
 *  logFile  - the FILE pointer to print to
 */
void executeCommand(TurtleSettings* settings, Command* command, FILE* logFile)
{
    char cmdName[MAX_CMD_NAME_SIZE + 1];
    double oldX, oldY, newX, newY;
    double deltaX, deltaY;

    strncpy(cmdName, command->name.value, MAX_CMD_NAME_SIZE + 1);
    if (strcmp(cmdName, "ROTATE") == 0)
    {
        rotate(settings, *((double*) command->value));
    }
    else if (strcmp(cmdName, "MOVE") == 0)
    {
        getPos(settings, &oldX, &oldY);
        move(settings, *((double*) command->value), &deltaX, &deltaY);
        getPos(settings, &newX, &newY);
        fprintf(logFile, LOG_FORMAT, cmdName, oldX, oldY, newX, newY);
        #ifdef PRINT_LOG
        fprintf(stderr, LOG_FORMAT, cmdName, oldX, oldY, newX, newY);
        #endif
    }
    else if (strcmp(cmdName, "DRAW") == 0)
    {
        getPos(settings, &oldX, &oldY);
        draw(settings, *((double*) command->value), &deltaX, &deltaY);
        getPos(settings, &newX, &newY);
        fprintf(logFile, LOG_FORMAT, cmdName, oldX, oldY, newX, newY);
        #ifdef PRINT_LOG
        fprintf(stderr, LOG_FORMAT, cmdName, oldX, oldY, newX, newY);
        #endif
    }
    else if (strcmp(cmdName, "FG") == 0)
    {
        settings->fgColour = *((int*) command->value);
        #ifndef NO_COLOURS
        setFgColour(settings->fgColour);
        #endif
    }
    else if (strcmp(cmdName, "BG") == 0)
    {
        settings->bgColour = *((int*) command->value);
        #ifndef NO_COLOURS
        setBgColour(settings->bgColour);
        #endif
    }
    else if (strcmp(cmdName, "PATTERN") == 0)
    {
        settings->pattern = *((char*) command->value);
    }
}

/**
 * Adds the angle from the commmand value to the angle setting in the TurtleSettings
 * struct.
 *
 * Parameters:
 *  settings - the TurtleSettings struct that contains the current settings
 *  angle    - the angle to add
 */
void rotate(TurtleSettings* settings, double angle)
{
    settings->angle += angle;
    settings->angle = adjustAngle(settings->angle);
}

/**
 * Moves the coordinates inside the TurtleSettings struct by a distance calculated
 * by polToRec.
 *
 * Parameters:
 *  settings - the current setting of the drawing
 *  distance - the magnitude of the polar vector
 *  deltaX   - (export) the distance to move in the 'x' direction
 *  deltaY   - (export) the distance to move in the 'y' direction
 */
void move(TurtleSettings* settings, double distance, double* deltaX, double* deltaY)
{
    /* Convert form polar coordinates to cartesian coordinates */
    polToRec(distance, settings->angle, deltaX, deltaY);

    /* Move current position */
    /* Take negative of deltaY as 'y' increases going down */
    settings->pos.x += *deltaX;
    settings->pos.y += -(*deltaY);
}

/**
 * Moves the coordinates inside the TurtleSettings struct by a distance calculated
 * by polToRec and draws a line from the old coordinates to the new coordinates.
 *
 * Parameters:
 *  settings - the TurtleSettings contain the current settings of the drawing
 *  distance - the magnitude of the polar vector
 *  deltaX   - (export) the distance to move in the 'x' direction
 *  deltaY   - (export) the distance to move in the 'y' direction
 */
void draw(TurtleSettings* settings, double distance, double* deltaX, double* deltaY)
{
    /* Save old positions to draw from */
    int oldX = (int) roundNum(settings->pos.x);
    int oldY = (int) roundNum(settings->pos.y);

    /* Move to new position */
    move(settings, distance, deltaX, deltaY);

    /* Adjust the deltas for correct drawing */
    adjustDeltas(deltaX, deltaY);
    /* Draw line in stdin */
    line(oldX, oldY, oldX + (int) (*deltaX), oldY + (int) -(*deltaY), &printPattern, &(settings->pattern));
}

/**
 * Returns true(non-zero) if the command name is "ROTATE", "MOVE", and "DRAW",
 * false(zero) otherwise
 */
int isCommandWithRealArg(char cmdName[])
{
    return strcmp(cmdName, "ROTATE") == 0 ||
           strcmp(cmdName, "MOVE") == 0 || strcmp(cmdName, "DRAW") == 0;
}

/**
 * Returns true(non-zero) if the command name is "FG", "BG", false(zero) otherwise
 */
int isCommandWithIntArg(char cmdName[])
{
    return strcmp(cmdName, "FG") == 0 || strcmp(cmdName, "BG") == 0;
}

/**
 * Returns true(non-zero) if the command name is "PATTERN", false(zero) otherwise
 */
int isCommandWithCharArg(char cmdName[])
{
    return strcmp(cmdName, "PATTERN") == 0;
}

/**
 * Returns true(non-zero) if the "FG" command's value if between 0 and 15 (inclusive)
 * or if the "BG" command's value is between 0 and 7, false(zero) otherwise.
 *
 * Parameters:
 *  cmdName  - the name of the command
 *  cmdValue - the value of the command
 * Returns:
 *  true(non-zero) if the command value is in a valid range, false(zero) otherwise
 */
int isOutOfBounds(char cmdName[], char cmdValue[])
{
    int isOutOfBounds = FALSE;
    int num = (int) strtol(cmdValue, NULL, 10);

    if (strcmp("FG", cmdName) == 0)
    {
        if (num < MIN_COL_CODE || num > MAX_FG_CODE)
        {
            isOutOfBounds = TRUE;
            fprintf(stderr, "ERROR: The %s command requires an integer between"
                            " %d and %d, not \"%s\".\n",
                    cmdName, MIN_COL_CODE, MAX_FG_CODE, cmdValue);
        }
    }
    else if (strcmp("BG", cmdName) == 0)
    {
        if (num < MIN_COL_CODE || num > MAX_BG_CODE)
        {
            isOutOfBounds = TRUE;
            fprintf(stderr, "ERROR: The %s command requires an integer between"
                            " %d and %d, not \"%s\".\n",
                    cmdName, MIN_COL_CODE, MAX_BG_CODE, cmdValue);
        }
    }

    return isOutOfBounds;
}

/**
 * Reduce the deltas by one, going towards zero. These will then draw the correct
 * amount of characters.
 *
 * Parameters:
 *  deltaX - (export) the number of spaces to draw in the x direction
 *  deltaY - (export) the number of spaces to draw in the y direction
 */
void adjustDeltas(double* deltaX, double* deltaY)
{
    if (*deltaX > 0)
    {
        (*deltaX)--;
    }
    else if (*deltaX < 0)
    {
        (*deltaX)++;
    }
    if (*deltaY > 0)
    {
        (*deltaY)--;
    }
    else if (*deltaY < 0)
    {
        (*deltaY)++;
    }
}

/**
 * Keeps the angle setting in the TurtleSettings struct between 0 and 360 as to
 * prevent overflow of the datatype.
 */
double adjustAngle(double angle)
{
    if (angle > 360)
    {
        angle -= 360;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    return angle;
}

/**
 * Prints a single character from the PATTERN command's value.
 *
 * Parameters:
 *  pattern - a void pointer pointing to the character to print
 */
void printPattern(void* pattern)
{
    if (pattern != NULL)
    {
        printf("%c", *((char*) pattern));
    }
}

/**
 * Frees the memory allocated to a Command struct
 *
 * Parameters:
 *  command - the command struct to free
 */
void freeCommand(void* command)
{
    Command* cmd = (Command*) command;
    free(cmd->value);
    cmd->value = NULL;

    free(command);
    command = NULL;
}
