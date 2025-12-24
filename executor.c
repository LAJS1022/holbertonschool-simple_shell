#include "shell.h"

/**
 * print_not_found - prints not found error
 * @cmd: command name
 */
void print_not_found(const char *cmd)
{
    fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
}

/**
 * execute_command - executes a command line
 * @line: command line
 * @interactive: 1 if interactive, 0 otherwise
 */
void execute_command(char *line, int interactive)
{
    pid_t pid;
    char *argv[64], *cmd_path, *line_copy;
    int i = 0;

    line_copy = strdup(line);
    if (line_copy == NULL)
        return;

    argv[i] = strtok(line_copy, " ");
    while (argv[i] != NULL && i < 63)
    {
        i++;
        argv[i] = strtok(NULL, " ");
    }
    if (argv[0] == NULL)
    {
        free(line_copy);
        return;
    }

    cmd_path = find_command(argv[0]);
    if (cmd_path == NULL)
    {
        print_not_found(argv[0]);
        /* liberar antes de salir */
        free(line_copy);
        if (!interactive)
        {
            /* no dejar nada pendiente */
            exit(127);
        }
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free(line_copy);
        return;
    }
    if (pid == 0)
    {
        if (execve(cmd_path, argv, environ) == -1)
        {
            perror(argv[0]);
            free(cmd_path);
            free(line_copy);
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }

    free(cmd_path);
    free(line_copy);
}
