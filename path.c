#include "shell.h"

/**
 * get_env_path - gets PATH from environment
 * Return: PATH string or NULL
 */
char *get_env_path(void)
{
    int i = 0;
    size_t prefix_len = 5;
    char *entry;

    if (environ == NULL)
        return (NULL);

    while (environ[i] != NULL)
    {
        entry = environ[i];
        if (strncmp(entry, "PATH=", prefix_len) == 0)
            return (entry + prefix_len);
        i++;
    }
    return (NULL);
}

/**
 * find_command - finds full path of a command
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_command(char *cmd)
{
    char *path_env, *path_copy, *dir, *full;
    struct stat st;
    size_t len_dir, len_cmd;

    if (cmd == NULL || *cmd == '\0')
        return (NULL);

    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0)
            return (strdup(cmd));
        return (NULL);
    }

    path_env = get_env_path();
    if (path_env == NULL || *path_env == '\0')
        return (NULL);

    path_copy = strdup(path_env);
    if (path_copy == NULL)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        len_dir = strlen(dir);
        len_cmd = strlen(cmd);
        full = malloc(len_dir + 1 + len_cmd + 1);
        if (full == NULL)
        {
            free(path_copy);
            return (NULL);
        }
        memcpy(full, dir, len_dir);
        full[len_dir] = '/';
        memcpy(full + len_dir + 1, cmd, len_cmd);
        full[len_dir + 1 + len_cmd] = '\0';

        if (stat(full, &st) == 0)
        {
            free(path_copy);
            return (full);
        }
        free(full);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}
