#include "shell.h"

/**
 * trim_spaces - removes leading/trailing spaces and tabs in-place
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

#define RBUF_SIZE 4096
#define LINE_SIZE 4096

/**
 * read_command - reads one logical line using read(), preserving leftovers
 * Behavior:
 * - Returns the entire trimmed line (without newline) as a newly allocated string.
 * - Preserves leftover bytes across calls (handles multiple lines per read()).
 * - Does NOT collapse internal spaces; only trims leading/trailing.
 * - If a line exceeds LINE_SIZE before newline, discards that line entirely.
 * - Uses only allowed functions (no getline/fgets/realloc).
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;
    static ssize_t rpos = 0;
    char line[LINE_SIZE];
    ssize_t linelen = 0;
    int discarding = 0;
    ssize_t i, nread;
    char *trimmed, *dup;

    for (;;)
    {
        for (i = rpos; i < rlen; i++)
        {
            char c = rbuf[i];

            if (c == '\n')
            {
                if (!discarding)
                {
                    line[linelen] = '\0';
                    trimmed = trim_spaces(line);
                    if (trimmed[0] != '\0')
                    {
                        dup = strdup(trimmed);
                        rpos = i + 1;
                        return (dup);
                    }
                }
                linelen = 0;
                discarding = 0;
                rpos = i + 1;
                continue;
            }

            if (!discarding)
            {
                if (linelen < (ssize_t)(LINE_SIZE - 1))
                    line[linelen++] = c;
                else
                    discarding = 1;
            }
        }

        rpos = 0;
        rlen = 0;

        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            if (!discarding && linelen > 0)
            {
                line[linelen] = '\0';
                trimmed = trim_spaces(line);
                if (trimmed[0] != '\0')
                {
                    dup = strdup(trimmed);
                    return (dup);
                }
            }
            if (isatty(STDIN_FILENO))
                printf("\n");
            return (NULL);
        }
        rlen = nread;
    }
}
