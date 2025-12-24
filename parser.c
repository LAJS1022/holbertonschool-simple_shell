#include "shell.h"

/**
 * trim_spaces - removes leading/trailing spaces in-place
 * @str: string to trim
 * Return: pointer to first non-space char (may be '\0')
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

#define BUF_SIZE 1024

/**
 * read_command - reads one line from stdin using read()
 * Return: newly allocated trimmed command string, or NULL on EOF/error
 */
char *read_command(void)
{
    static char buf[BUF_SIZE];
    ssize_t nread, i;
    char *trimmed, *dup;

    nread = read(STDIN_FILENO, buf, BUF_SIZE - 1);
    if (nread <= 0)
    {
        if (isatty(STDIN_FILENO))
            printf("\n");
        return (NULL);
    }

    buf[nread] = '\0';

    /* find newline and cut there */
    for (i = 0; i < nread; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = '\0';
            break;
        }
    }

    trimmed = trim_spaces(buf);
    if (trimmed[0] == '\0')
    {
        /* empty line: try again */
        return (read_command());
    }

    dup = strdup(trimmed);
    return (dup);
}
