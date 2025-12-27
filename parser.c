#include "shell.h"
#include <string.h>
#include <stdlib.h>

char **parse_line(char *line)
{
    char **argv;
    char *token;
    int bufsize = 64, i = 0;

    argv = malloc(bufsize * sizeof(char *));
    if (!argv)
        return NULL;

    token = strtok(line, " \t\r\n");
    while (token)
    {
        argv[i++] = token;
        if (i >= bufsize)
        {
            bufsize += 64;
            argv = realloc(argv, bufsize * sizeof(char *));
            if (!argv)
                return NULL;
        }
        token = strtok(NULL, " \t\r\n");
    }
    argv[i] = NULL;
    return argv;
}
