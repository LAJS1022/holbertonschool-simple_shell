#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * resolve_path - find full path of command
 * @cmd: command name
 *
 * Return: full path string (malloc'd) or NULL
 */
char *resolve_path(char *cmd)
{
    char *path = NULL, *dir, *full;
    size_t len;
    int i;

    if (cmd == NULL)
        return (NULL);

    if (cmd[0] == '/' || strncmp(cmd, "./", 2) == 0 || strncmp(cmd, "../", 3) == 0)
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        return (NULL);
    }

    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path = environ[i] + 5;
            break;
        }
    }

    if (!path || strlen(path) == 0)
        return (NULL);

    path = strdup(path);
    if (!path)
        return (NULL);

    dir = strtok(path, ":");
    while (dir != NULL)
    {
        len = strlen(dir) + strlen(cmd) + 2;
        full = malloc(len);
        if (!full)
        {
            free(path);
            return (NULL);
        }

        strcpy(full, dir);
        strcat(full, "/");
        strcat(full, cmd);

        if (access(full, X_OK) == 0)
        {
            free(path);
            return (full);
        }

        free(full);
        dir = strtok(NULL, ":");
    }

    free(path);
    return (NULL);
}
