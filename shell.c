#include "shell.h"

void print_prompt(void)
{
    printf("#cisfun$ ");
}

char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
    {
        *end = '\0';
        end--;
    }

    return str;
}

char *read_command(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *trimmed;
    char *result;

    read = getline(&line, &len, stdin);
    if (read == -1)
    {
        free(line);
        if (isatty(STDIN_FILENO))
            printf("\n");
        return (NULL);
    }

    line[strcspn(line, "\n")] = '\0';
    trimmed = trim_spaces(line);

    if (trimmed[0] == '\0')
    {
        free(line);
        return (read_command());
    }

    result = strdup(trimmed);
    free(line);
    return result;
}

char *get_env_path(void)
{
    int i = 0;
    size_t prefix_len = 5;
    char *entry;

    while (environ[i] != NULL)
    {
        entry = environ[i];
        if (strncmp(entry, "PATH=", prefix_len) == 0)
            return entry + prefix_len;
        i++;
    }
    return NULL;
}

char *find_command(char *cmd)
{
    char *path_env;
    char *path_copy;
    char *dir;
    char *full;
    struct stat st;
    size_t len_dir, len_cmd;

    if (cmd == NULL || *cmd == '\0')
        return NULL;

    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0)
            return strdup(cmd);
        return NULL;
    }

    path_env = get_env_path();
    if (path_env == NULL || *path_env == '\0')
        return NULL;

    path_copy = strdup(path_env);
    if (path_copy == NULL)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        len_dir = strlen(dir);
        len_cmd = strlen(cmd);
        full = malloc(len_dir + 1 + len_cmd + 1);
        if (full == NULL)
        {
            free(path_copy);
            return NULL;
        }

        memcpy(full, dir, len_dir);
        full[len_dir] = '/';
        memcpy(full + len_dir + 1, cmd, len_cmd);
        full[len_dir + 1 + len_cmd] = '\0';

        if (stat(full, &st) == 0)
        {
            free(path_copy);
            return full;
        }

        free(full);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

void execute_command(char *line)
{
    pid_t pid;
    char *argv[64];
    int i = 0;
    char *cmd_path;

    argv[i] = strtok(line, " ");
    while (argv[i] != NULL && i < 63)
    {
        i++;
        argv[i] = strtok(NULL, " ");
    }

    if (argv[0] == NULL)
        return;

    cmd_path = find_command(argv[0]);
    if (cmd_path == NULL)
    {
        fprintf(stderr, "%s: command not found\n", argv[0]);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        return;
    }
    if (pid == 0)
    {
        if (execve(cmd_path, argv, environ) == -1)
        {
            perror(argv[0]);
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
    free(cmd_path);
}

int main(void)
{
    char *line;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            print_prompt();

        line = read_command();
        if (line == NULL)
            break;

        execute_command(line);
        free(line);
    }
    return 0;
}
