#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * run_shell - main loop of the shell
 *
 * Return: exit status of last executed command
 */
int run_shell(void)
{
    char *line = NULL, *args[64], *path_cmd = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    int last_status = 0;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, ":) ", 3);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            return (last_status);
        }

        tokenize(line, args);

        if (args[0] == NULL)
            continue;

        path_cmd = resolve_path(args[0]);

        if (path_cmd != NULL && access(path_cmd, X_OK) == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                execve(path_cmd, args, environ);
                perror("execve");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                wait(&last_status);
                if (WIFEXITED(last_status))
                    last_status = WEXITSTATUS(last_status);
            }
        }
        else
        {
            write(STDERR_FILENO, "./hsh: 1: ", 10);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, ": not found\n", 12);
            last_status = 127;
        }

        free(path_cmd);
    }
}
