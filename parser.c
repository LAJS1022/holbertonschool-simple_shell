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
 * read_command - reads one line from stdin safely (no getline)
 * Return: newly allocated command string or NULL on EOF/error
 */
char *read_command(void)
{
    char buf[1024];
    char *trimmed, *dup;

    if (fgets(buf, sizeof(buf), stdin) == NULL)
    {
        if (isatty(STDIN_FILENO))
            printf("\n");
        return (NULL);
    }

    /* remove trailing newline if present */
    buf[strcspn(buf, "\n")] = '\0';

    trimmed = trim_spaces(buf);
    if (trimmed[0] == '\0')
    {
      /* empty line: read next line recursively until EOF */
        return (read_command());
    }

    dup = strdup(trimmed);
    return (dup);
}
