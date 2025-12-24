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

#define RBUF_SIZE 4096
#define LINE_SIZE 4096

/**
 * read_command - reads exactly one logical line using read() with leftovers
 * Behavior:
 * - Returns one trimmed non-empty line per call.
 * - Preserves leftover bytes across calls (handles multiple lines from one read).
 * - Skips empty lines after trimming.
 * - If a line exceeds LINE_SIZE before newline, discards the entire line.
 * - Uses only allowed functions: read, memcpy, strdup, etc. (no getline/fgets/realloc).
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;  /* bytes currently in rbuf */
    static ssize_t rpos = 0;  /* current read position in rbuf */
    char line[LINE_SIZE];
    ssize_t linelen = 0;
    int discarding = 0;       /* if current logical line overflowed */
    ssize_t i, nread;
    char *trimmed, *dup;

    for (;;)
    {
        /* Consume buffered bytes until newline or end */
        for (i = rpos; i < rlen; i++)
        {
            if (rbuf[i] == '\n')
            {
                /* End of logical line */
                if (!discarding)
                {
                    line[linelen] = '\0';
                    trimmed = trim_spaces(line);
                    if (trimmed[0] != '\0')
                    {
                        dup = strdup(trimmed);
                        /* Advance past newline and return one command */
                        rpos = i + 1;
                        return (dup);
                    }
                }
                /* Reset for next logical line */
                linelen = 0;
                discarding = 0;
                rpos = i + 1;
                /* Continue scanning for another (possibly in same buffer) */
                continue;
            }

            /* Regular byte */
            if (!discarding)
            {
                if (linelen < (ssize_t)(LINE_SIZE - 1))
                {
                    line[linelen++] = rbuf[i];
                }
                else
                {
                    /* Line exceeded capacity: discard until newline */
                    discarding = 1;
                }
            }
        }

        /* Buffer exhausted: reset to trigger refill */
        rpos = 0;
        rlen = 0;

        /* Read more data */
        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            /* EOF or error: if partial line without newline, consider it */
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
            /* Nothing usable; finish */
            if (isatty(STDIN_FILENO))
                printf("\n");
            return (NULL);
        }
        rlen = nread;
        /* Loop to process newly read bytes, preserving leftovers */
    }
}
