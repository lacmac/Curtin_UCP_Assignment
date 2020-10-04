#include <stdio.h>
#include <stdlib.h>
#include "turtleGraphics.h"

/* Number of expected command line arguments */
#define NUM_ARGS 2

/**
 * Parameters:
 *  argc - two
 *  argv - executableName, input fileName
 * Returns:
 *  An error code for a corresponding error, please see fileIO.c:15 for details
 */
int main(int argc, char* argv[])
{
    int errNo;
    char* fileName;
    LinkedList* cmdList;
    int isFileValid;

    errNo = 0;

    if (argc == NUM_ARGS)
    {
        fileName = argv[1];
        /* Returns zero on success, program will exit if a line is invalid */
        isFileValid = validateInputFile(fileName);
        if (isFileValid == 0)
        {
            cmdList = NULL;
            /* Read all commands from the file into a Linked List */
            cmdList = readCommandsFromFile(fileName);
            if (cmdList != NULL)
            {
                /* Sets the colours for TurtleGraphicsSimple */
                #ifdef NO_COLOURS
                setColoursSimple();
                #endif
                clearScreen();
                executeCommands(cmdList);
                resetColours();
                penDown();
                /* Free a generic Linked List with a function pointer to free
                 * the data inside a node */
                LinkedList_free(cmdList, &freeCommand);
            }
            else
            {
                fprintf(stderr, "ERROR: Could not create the linked list.\n");
            }
        }
        else
        {
            errNo = isFileValid;
            fprintf(stderr, "ERROR: The input file is invalid. ");
            fprintf(stderr, "Please re-run the program with a valid input file.\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Invalid number of arguments. ");
        fprintf(stderr, "Usage: ./TurtleGraphics <fileName>\n");
    }

    return errNo;
}

/**
 * Iterates through each command in the Linked List and executes them. Printing
 * to the log file occurs at every DRAW and MOVE command.
 *
 * Parameters:
 *  cmdList - the Linked List of commands to execute
 */
void executeCommands(LinkedList* cmdList)
{
    Command* command;
    TurtleSettings* settings;
    FILE* logFile;
    int isInBounds;

    settings = NULL;
    /* Create a settings struct to keep track of the graphics parameters */
    settings = createSettings();
    logFile = NULL;
    logFile = fopen("graphics.log", "a");
    if (logFile != NULL && settings != NULL)
    {
        fprintf(logFile, "---\n");
        /* The program should exit when the x or y coordinate goes out of the
         * terminal bounds */
        isInBounds = TRUE;
        while (!LinkedList_isEmpty(cmdList) && isInBounds)
        {
            /* Check the current cursor coordinate is valid */
            if (roundNum(settings->pos.x) >= 0 && roundNum(settings->pos.y) >= 0)
            {
                /* Much quicker to remove the first Command instead of using get */
                command = (Command*) LinkedList_removeFirst(cmdList);
                executeCommand(settings, command, logFile);
                /* removeFirst returns a malloc'ed Command*, so free it */
                freeCommand(command);
            }
            else
            {
                isInBounds = FALSE;
                penDown();
                /* Flush output so the cursor moves down before printing error */
                fflush(stdout);
                fprintf(stderr, "ERROR: Invalid drawing. Cursor position is not valid.\n");
            }
        }

        /* Check if the file closed successfully */
        if (fclose(logFile) != 0)
        {
            perror("ERROR: The file was not closed successfully");
        }
    }
    else
    {
        perror("ERROR: The log file could not be opened");
    }

    /*Free allocated memory */
    free(settings);
    settings = NULL;
}
