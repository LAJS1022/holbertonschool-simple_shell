#include "shell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

extern char **environ;

/**
 * execute - ejecuta un comando
 * @argv: argumentos del comando
 */
void execute(char **argv)
{
    char *resolved;
    pid_t pid;
    int status;

    resolved = find_command(argv[0]);
    if (!resolved)
    {
        fprintf(stderr, "%s: not found\n", argv[0]);
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        execve(resolved, argv, environ);
        perror("execve");
        _exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free(resolved);
    }
    else
    {
        perror("fork");
        free(resolved);
    }
}
