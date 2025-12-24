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
 * read_command - reads one logical line using read(), robust to huge inputs
 * Behavior:
 * - Returns exactly one trimmed non-empty line per call.
 * - Skips empty lines (after trimming) and continues reading.
 * - If a line exceeds LINE_SIZE before newline, it is discarded entirely
 *   (consumes until the newline) and not returned as a partial command.
 * - Returns NULL on EOF or read error (prints newline if interactive).
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;  /* bytes in rbuf */
    static ssize_t rpos = 0;  /* current position in rbuf */
    char line[LINE_SIZE];
    ssize_t linelen = 0;
    ssize_t i, nread;
    char *trimmed, *dup;
    int discarding = 0; /* if current logical line overflowed and is being discarded */

    for (;;)
    {
        /* Consume from rbuf until newline or buffer end */
        for (i = rpos; i < rlen; i++)
        {
            if (rbuf[i] == '\n')
            {
                /* End of logical line */
                if (!discarding)
                {
                    /* Finalize collected line */
                    line[linelen] = '\0';
                    trimmed = trim_spaces(line);
                    if (trimmed[0] != '\0')
                    {
                        dup = strdup(trimmed);
                        /* Advance past newline for next call */
                        rpos = i + 1;
                        return (dup);
                    }
                }
                /* Reset for next logical line */
                linelen = 0;
                discarding = 0;
                rpos = i + 1;
                /* Continue to find next non-empty logical line */
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
                    /* Line exceeded capacity: start discarding until newline */
                    discarding = 1;
                }
            }
        }

        /* Consumed entire buffer; reset positions to trigger refill */
        rpos = 0;
        rlen = 0;

        /* Refill buffer */
        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            /* EOF or error: if we have a partial line without newline */
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
        /* Loop to process newly read bytes */
    }
}
