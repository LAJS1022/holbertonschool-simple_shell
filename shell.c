#include "shell.h"

void print_prompt(void)
{
    printf(":) ");
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

char *find_command(char *cmd)
{
    char *path, *dir, *full;
    struct stat st;

    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0)
            return strdup(cmd);
        return NULL;
    }

    path = getenv("PATH");
    if (!path)
        return NULL;

    path = strdup(path);
    dir = strtok(path, ":");
    while (dir)
    {
        full = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full)
        {
            free(path);
            return NULL;
        }
        sprintf(full, "%s/%s", dir, cmd);
        if (stat(full, &st) == 0)
        {
            free(path);
            return full;
        }
        free(full);
        dir = strtok(NULL, ":");
    }
    free(path);
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

    cmd_path = find_command(argv[0]);
    if (!cmd_path)
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
