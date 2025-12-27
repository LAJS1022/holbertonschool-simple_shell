#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * main - punto de entrada del shell
 * Return: siempre 0
 */
int main(void)
{
    char *line;
    size_t len;
    ssize_t read;
    char **argv;

    line = NULL;
    len = 0;

    while (1)
    {
        printf("$ ");
        read = getline(&line, &len, stdin);
        if (read == -1)
            break;

        argv = parse_line(line);
        if (argv && argv[0])
            execute(argv);

        free(argv);
    }

    free(line);
    return (0);
}
