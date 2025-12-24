#include "shell.h"

/**
 * trim_spaces - removes leading/trailing spaces
 * @str: string to trim
 * Return: trimmed string (in-place)
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
 * read_command - reads one line from stdin using read()
 * Return: newly allocated command string or NULL on EOF/error
 */
char *read_command(void)
{
    char buf[1024];
    ssize_t nread;
    char *trimmed, *dup;

    nread = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    if (nread <= 0)
    {
        if (isatty(STDIN_FILENO))
            printf("\n");
        return (NULL);
    }

    buf[nread] = '\0';

    /* remove trailing newline if present */
    buf[strcspn(buf, "\n")] = '\0';

    trimmed = trim_spaces(buf);
    if (trimmed[0] == '\0')
    {
        /* empty line: try again */
        return (read_command());
    }

    dup = strdup(trimmed);
    return (dup);
}
