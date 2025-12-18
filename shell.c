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

void execute_command(char *line)
{
    pid_t pid;
    char *argv[2];

    argv[0] = line;
    argv[1] = NULL;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        if (execve(argv[0], argv, environ) == -1)
        {
            perror(argv[0]);
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
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
    return (0);
}
