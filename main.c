#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **argv;

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
    return 0;
}
