#include "shell.h"

static const char *g_progname = "hsh";

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
        return NULL;
    }

    line[strcspn(line, "\n")] = '\0';
    trimmed = trim_spaces(line);

    if (trimmed[0] == '\0')
    {
        free(line);
        return read_command();
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

    if (environ == NULL)
        return NULL;

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
    char *path_env, *path_copy, *dir, *full;
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

void print_not_found(const char *cmd)
{
    fprintf(stderr, "%s: 1: %s: not found\n", g_progname, cmd);
}

void execute_command(char *line, int interactive)
{
    pid_t pid;
    char *argv[64];
    int i = 0;
    char *cmd_path;
    char *line_copy;

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
        free(line_copy);
        if (!interactive)
            exit(127);
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

int main(int argc, char **argv)
{
    char *line;
    int interactive;

    (void)argc;

    if (argv != NULL && argv[0] != NULL)
        g_progname = argv[0];

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            print_prompt();

        line = read_command();
        if (line == NULL)
            break;

        execute_command(line, interactive);
        free(line);
    }
    return 0;
}
