#include "shell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

extern char **environ;

void execute(char **argv)
{
    char *resolved = find_command(argv[0]);
    if (!resolved)
    {
        fprintf(stderr, "%s: not found\n", argv[0]);
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        execve(resolved, argv, environ);
        perror("execve");
        _exit(127);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        free(resolved); /* liberar en el padre */
    }
    else
    {
        perror("fork");
        free(resolved);
    }
}
