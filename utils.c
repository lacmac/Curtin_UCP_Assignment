#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include "utils.h"

/**
 * Converts the character array to uppercase for easier comparison.
 *
 * Parameters:
 *  string (export) - the character array to convert to uppercase
 */
void convertToUpperCase(char string[])
{
    int ii;
    for (ii = 0; ii < strlen(string); ii++)
    {
        string[ii] = (char) toupper(string[ii]);
    }
}

/**
 * Checks if a character array is a real number of type double. Uses 'strtod' to
 * verify that it is a double and HUGE_VAL to check for overflow of the datatype.
 *
 * Parameters:
 *  cmdValue - the character array to check
 * Returns:
 *  true(non-zero) if it is a valid double, or false(zero) otherwise
 */
int isReal(char cmdValue[])
{
    int isReal;
    char* err;
    double num;

    isReal = TRUE;
    num = strtod(cmdValue, &err);
    /* Overflow protection or NaN */
    if (num == HUGE_VAL || num == -HUGE_VAL || *err != 0)
    {
        isReal = FALSE;
    }

    return isReal;
}

/**
 * Checks if a character array is an integer. Uses 'strtol' to verify that it is
 * an integer and INT_MAX and INT_MIN to check for overflow of the datatype.
 *
 * Parameters:
 *  cmdValue - the character array to check
 * Returns:
 *  true(non-zero) if it is a valid integer, or false(zero) otherwise
 */
int isInteger(char cmdValue[])
{
    int isInteger;
    char* err;
    int num;

    isInteger = TRUE;
    num = (int) strtol(cmdValue, &err, 10);
    /* Overflow protection or NaN */
    if (num == INT_MIN || num == INT_MAX || *err != 0)
    {
        isInteger = FALSE;
    }

    return isInteger;
}

/**
 * Checks if a character array is a single character. Verifies this by checking
 * the length of the array and if it a whitespace character or not.
 * Sidenote, this could probably be abused.
 *
 * Parameters:
 *  cmdValue - the character array to check
 * Returns:
 *  true(non-zero) if the string length is one and that character is not a whitespace
 *  character, false(zero) otherwise
 */
int isCharacter(char cmdValue[])
{
    /* A character is a string of length one that is not a whitespace character */
    return strlen(cmdValue) == 1 && !isspace(cmdValue[0]);
}

/**
 * Converts from polar coordinates to cartesian coordinates.
 *
 * Parameters:
 *  d          - magnitude of the polar coords
 *  angle      - angle of the polar coords in degrees
 *  x (export) - the x-coordinate equivalent
 *  y (export) - the y-coordinate equivalent
 */
void polToRec(double d, double angle, double* x, double* y)
{
    /* sin and cos take in radians not degrees so times angle by PI/180 */
    *x = d * cos(angle * M_PI / 180.0);
    *y = d * sin(angle * M_PI / 180.0);
}

/**
 * Obtained from stackoverflow.com question 497018.
 * Rounds a real number of data-type double to the nearest whole number.
 *
 * Parameters:
 *  n - the number to round
 * Returns:
 *  roundedNum - 'n' rounded to the nearest whole number
 */
double roundNum(double n)
{
    return n > 0.0 ? floor(n + 0.5) : ceil(n - 0.5);
}

/**
 * Removes a newline character from the end of a string of length greater than 1.
 *
 * Parameters:
 *  str - the string to remove the newline character from
 */
void removeNewLineChar(char* str)
{
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = '\0';
    }
}
