#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 * exec_cmd - execute a command if found in PATH
 * @cmd: full path to command
 * @argv: argument vector
 *
 * Return: 0 on success, -1 on failure
 */
int exec_cmd(char *cmd, char **argv)
{
    if (cmd == NULL)
    {
        write(STDERR_FILENO, argv[0], strlen(argv[0]));
        write(STDERR_FILENO, ": command not found\n", 21);
        return (-1);
    }

    if (execve(cmd, argv, environ) == -1)
    {
        perror(argv[0]);
        return (-1);
    }

    return (0);
}
