#include "shell.h"
#include <string.h>

/**
 * tokenize - split line into arguments
 * @line: input string
 * @args: array to fill
 */
void tokenize(char *line, char **args)
{
    char *token;
    int i = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && i < 63)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}
