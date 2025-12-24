#include "shell.h"

/**
 * trim_spaces - removes leading/trailing spaces in-place
 * @str: string to trim
 * Return: pointer to the first non-space char (may be '\0')
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

/*
 * Internal buffered reader using read():
 * - Accumulates data from stdin.
 * - Returns exactly one line per call (without '\n').
 * - Preserves leftover bytes for subsequent calls.
 */
#define RBUF_SIZE 4096

/**
 * read_command - reads one logical line using an internal buffer with read()
 * Return: newly allocated trimmed command string, or NULL on EOF/error
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;   /* bytes currently in rbuf */
    static ssize_t rpos = 0;   /* current read position in rbuf */
    char line[RBUF_SIZE];
    ssize_t linelen = 0;
    ssize_t i;
    ssize_t nread;
    char *trimmed, *dup;

    /* Keep reading until we can return one line or reach EOF */
    while (1)
    {
        /* Consume from rbuf until newline or buffer end */
        for (i = rpos; i < rlen; i++)
        {
            if (rbuf[i] == '\n')
            {
                /* Copy bytes [rpos, i) as a line (exclude '\n') */
                ssize_t chunk = i - rpos;

                if (chunk > 0)
                {
                    if (linelen + chunk >= (ssize_t)(sizeof(line) - 1))
                        chunk = (ssize_t)(sizeof(line) - 1 - linelen);
                    memcpy(line + linelen, rbuf + rpos, (size_t)chunk);
                    linelen += chunk;
                }
                line[linelen] = '\0';

                /* Advance past '\n' */
                rpos = i + 1;

                trimmed = trim_spaces(line);
                if (trimmed[0] == '\0')
                {
                    /* Empty (after trim): reset line buffer and continue */
                    linelen = 0;
                    continue;
                }

                dup = strdup(trimmed);
                return (dup);
            }
        }

        /* No newline in current buffer: append remaining bytes to line */
        if (rpos < rlen)
        {
            ssize_t chunk = rlen - rpos;

            if (linelen + chunk >= (ssize_t)(sizeof(line) - 1))
                chunk = (ssize_t)(sizeof(line) - 1 - linelen);

            if (chunk > 0)
            {
                memcpy(line + linelen, rbuf + rpos, (size_t)chunk);
                linelen += chunk;
            }
            rpos += (rlen - rpos); /* consumed all */
        }

        /* If we've exhausted rbuf, refill it */
        rpos = 0;
        rlen = 0;

        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            /* EOF or error: if we have a partial line, return it; else NULL */
            if (linelen > 0)
            {
                line[linelen] = '\0';
                trimmed = trim_spaces(line);
                if (trimmed[0] == '\0')
                {
                    /* Partial but empty: treat as EOF */
                    if (isatty(STDIN_FILENO))
                        printf("\n");
                    return (NULL);
                }
                dup = strdup(trimmed);
                return (dup);
            }
            if (isatty(STDIN_FILENO))
                printf("\n");
            return (NULL);
        }
        rlen = nread;
        rpos = 0;
    }
}
