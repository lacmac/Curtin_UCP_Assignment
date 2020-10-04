#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"

#define MAX_LINE_SIZE 50

/**
 * Reads an input file and verifies that each line is valid. If a line is
 * invalid the file stops being read and the function returns an error number.
 *
 * Parameters:
 *   fileName - the name of the file to validate
 * Returns:
 *   0 - on success
 *   1 - if the file could not be opened
 *   2 - if the file could not be closed
 *   3 - if their is a system error while reading the file
 *   4 - if the file is empty
 *   5 - if the number of parameters on all lines does not equal two
 *   6 - if the specified command does not exit
 *   7 - if the data type does not match the one required by the command
 *   8 - if the data type is out of the valid range
 */
int validateInputFile(char* fileName)
{
    int errNo;
    FILE* cmdFile;
    char line[MAX_LINE_SIZE + 1];
    int isEmpty;

    errNo = 0;

    cmdFile = NULL;
    cmdFile = fopen(fileName, "r");
    if (cmdFile != NULL)
    {
        /* isEmpty is set to FALSE when a line inside the file is not empty */
        isEmpty = TRUE;
        /* Fetch each line */
        while (fgets(line, MAX_LINE_SIZE + 1, cmdFile) != NULL && errNo == 0)
        {
            if (!ferror(cmdFile))
            {
                /* Returns zero on success */
                errNo = validateLine(line, &isEmpty);
            }
            else
            {
                errNo = 3; /* IO Error */
                perror("ERROR: An IO error occurred while reading from the file");
            }
        }
        if (isEmpty)
        {
            errNo = 4; /* Input file is empty */
            fprintf(stderr, "ERROR: The input file is empty.\n");
        }
        if (fclose(cmdFile) != 0)
        {
            errNo = 2; /* Error closing file */
            perror("ERROR: The file was not closed successfully");
        }
    }
    else
    {
        errNo = 1; /* File could not be opened */
        perror("ERROR: The file could not be opened");
    }

    return errNo;
}

/**
 * Validates a Command from a line retrieved from the input file. Returns an error
 * code when the line is invalid.
 *
 * Parameters:
 *  line    - the line to validate
 *  isEmpty - (export) whether or not the line is empty
 * Returns:
 *   5 - if the number of parameters on all lines does not equal two
 *   6 - if the specified command does not exit
 *   7 - if the data type does not match the one required by the command
 *   8 - if the data type is out of the valid range
 */
int validateLine(char line[], int* isEmpty)
{
    int errNo;
    char cmdName[MAX_CMD_NAME_SIZE + 1], cmdNameUC[MAX_CMD_NAME_SIZE + 1];
    char cmdValue[MAX_CMD_PARAM_SIZE + 1], cmdParamTemp[MAX_CMD_PARAM_SIZE + 1];
    int numParams;

    errNo = 0;

    removeNewLineChar(line);
    /* If the line is not empty */
    if (strcmp(line, "") != 0)
    {
        (*isEmpty) = FALSE;
        /* cmdParamTemp is used to test if there is more than two params */
        numParams = sscanf(line, " %10s %50s %50s ", cmdName, cmdValue, cmdParamTemp);
        if (numParams == 2)
        {
            strncpy(cmdNameUC, cmdName, MAX_CMD_NAME_SIZE + 1);
            convertToUpperCase(cmdNameUC);
            /* If the command is ROTATE, DRAW or MOVE */
            if (isCommandWithRealArg(cmdNameUC))
            {
                if (!isReal(cmdValue))
                {
                    errNo = 7; /* Not the required data type */
                    fprintf(stderr, "ERROR: The %s command requires a double or "
                                    "float, not \"%s\".\n", cmdName, cmdValue);
                }
            }
            /* If the command is FG or BG */
            else if (isCommandWithIntArg(cmdNameUC))
            {
                if (!isInteger(cmdValue))
                {
                    errNo = 7; /* Not the required data type */
                    fprintf(stderr, "ERROR: The %s command requires an integer, "
                                    "not \"%s\".\n", cmdName, cmdValue);
                }
                else if (isOutOfBounds(cmdNameUC, cmdValue))
                {
                    errNo = 8; /* Integer is out of the valid range */
                }
            }
            /* If the command is PATTERN */
            else if (isCommandWithCharArg(cmdNameUC))
            {
                if (!isCharacter(cmdValue))
                {
                    errNo = 7; /* Not the required data type */
                    fprintf(stderr, "ERROR: The PATTERN command requires a single"
                                    " character, not \"%s\".\n", cmdValue);
                }
            }
            else
            {
                errNo = 6; /* Invalid command name */
                fprintf(stderr, "ERROR: The \"%s\" command does not exist.\n", cmdName);
                fprintf(stderr, "Use one or more of the following commands instead:"
                                " ROTATE, MOVE, DRAW, FG, BG, PATTERN.\n");
            }
        }
        else
        {
            errNo = 5; /* Incorrect number of params */
            fprintf(stderr, "ERROR: The line \"%s\" has an incorrect number of "
                            "parameters.\n", line);
        }
    }

    return errNo;
}

/**
 * Opens a file with the specified file name and reads each line into a Command
 * struct which is inserted into a Linked List that is then returned.
 *
 * Parameters:
 *  fileName - the name of the file to read the Commands from
 * Returns:
 *  cmdList - the Linked List of Command structs
 */
LinkedList* readCommandsFromFile(char* fileName)
{
    FILE* cmdFile;
    LinkedList* cmdList;
    char line[MAX_LINE_SIZE + 1];

    cmdList = NULL;
    cmdFile = NULL;
    cmdFile = fopen(fileName, "r");
    if (cmdFile != NULL)
    {
        cmdList = LinkedList_create();
        /* Fetch each line in the file */
        while (fgets(line, MAX_LINE_SIZE + 1, cmdFile) != NULL)
        {
            if (!ferror(cmdFile))
            {
                /* Convert each line to a Command struct and insert it into the list */
                processLine(cmdList, line);
            }
            else
            {
                perror("ERROR: An IO error occurred while reading from the file");
            }
        }

        if (fclose(cmdFile) != 0)
        {
            perror("ERROR: The file was not closed successfully");
        }
    }
    else
    {
        perror("ERROR: The file could not be opened");
    }

    return cmdList;
}

/**
 * Converts a valid line containing a command into a Command struct which is then
 * inserted into the end of the Linked List.
 *
 * Parameters:
 *  cmdList - the Linked List to insert the commands into
 *  line    - the character array to read the command from
 */
void processLine(LinkedList* cmdList, char line[])
{
    char cmdName[MAX_CMD_NAME_SIZE + 1], cmdValue[MAX_CMD_PARAM_SIZE + 1];
    int cmdValueInt;
    char cmdValueChar;
    double cmdValueReal;

    /* if the line is not empty */
    if (strcmp(line, "\n") != 0)
    {
        /* Retrieve the name of the command and its value */
        sscanf(line, " %50s %50s ", cmdName, cmdValue);
        /* Convert the name to uppercase for easier comparison */
        convertToUpperCase(cmdName);
        /* If the command is ROTATE, DRAW or MOVE */
        if (isCommandWithRealArg(cmdName))
        {
            cmdValueReal = strtod(cmdValue, NULL);
            LinkedList_insertLast(cmdList, createCommand(cmdName, MAX_CMD_NAME_SIZE + 1, &cmdValueReal));
        }
        /* If the command is FG, or BG */
        else if (isCommandWithIntArg(cmdName))
        {
            cmdValueInt = (int) strtol(cmdValue, NULL, 10);
            LinkedList_insertLast(cmdList, createCommand(cmdName, MAX_CMD_NAME_SIZE + 1, &cmdValueInt));
        }
        /* If the command is PATTERN */
        else if (isCommandWithCharArg(cmdName))
        {
            cmdValueChar = cmdValue[0];
            LinkedList_insertLast(cmdList, createCommand(cmdName, MAX_CMD_NAME_SIZE + 1, &cmdValueChar));
        }
    }
}
