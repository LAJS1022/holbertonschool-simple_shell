#include "shell.h"

/**
 * trim_spaces - removes leading/trailing spaces
 * @str: string to trim
 * Return: trimmed string
 */
char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '\0')
        return (str);

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
    {
        *end = '\0';
        end--;
    }
    return (str);
}

/**
 * read_command - reads a line from stdin
 * Return: command string or NULL
 */
char *read_command(void)
{
    char *line = NULL, *trimmed, *result;
    size_t len = 0;
    ssize_t read;

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
    return (result);
}
