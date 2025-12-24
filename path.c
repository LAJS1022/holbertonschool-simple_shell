#include "shell.h"
#include <sys/stat.h>

extern char **environ;

/**
 * find_command - resolves command path
 * @cmd: command name
 * Return: allocated string with path or NULL if not found
 */
char *find_command(const char *cmd)
{
    char *path = NULL, *token, *full;
    struct stat st;
    int i;

    /* If command is absolute or relative path */
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (stat(cmd, &st) == 0)
            return strdup(cmd);
        return NULL;
    }

    /* Search PATH in environ manually */
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path = strdup(environ[i] + 5);
            break;
        }
    }
    if (!path)
        return NULL;

    token = strtok(path, ":");
    while (token)
    {
        full = malloc(strlen(token) + strlen(cmd) + 2);
        if (!full)
        {
            free(path);
            return NULL;
        }
        sprintf(full, "%s/%s", token, cmd);
        if (stat(full, &st) == 0)
        {
            free(path);
            return full;
        }
        free(full);
        token = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}
