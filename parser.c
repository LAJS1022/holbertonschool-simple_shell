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

/**
 * read_command - reads one logical line using read() with dynamic buffering
 * Behavior:
 * - Returns exactly one trimmed non-empty line per call.
 * - Skips empty lines (after trimming) and continues reading.
 * - Handles arbitrarily large input lines by growing a heap buffer.
 * - Returns NULL on EOF/error (prints newline if interactive).
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;  /* bytes currently in rbuf */
    static ssize_t rpos = 0;  /* current read position in rbuf */
    char *line = NULL;
    size_t cap = 0;
    size_t len = 0;
    ssize_t i, nread;
    char *trimmed, *dup;

    for (;;)
    {
        /* Consume from rbuf until newline or buffer end */
        for (i = rpos; i < rlen; i++)
        {
            if (rbuf[i] == '\n')
            {
                /* finalize collected bytes as a line (exclude '\n') */
                if (line == NULL)
                {
                    /* empty line buffer to this point */
                    line = malloc(1);
                    if (line == NULL)
                        return (NULL);
                    line[0] = '\0';
                    cap = 1;
                    len = 0;
                }
                else
                {
                    /* ensure NUL terminator */
                    if (len == cap)
                    {
                        char *tmp = realloc(line, cap + 1);

                        if (tmp == NULL)
                        {
                            free(line);
                            return (NULL);
                        }
                        line = tmp;
                        cap += 1;
                    }
                    line[len] = '\0';
                }

                /* advance past '\n' for next call */
                rpos = i + 1;

                /* trim and decide */
                trimmed = trim_spaces(line);
                if (trimmed[0] != '\0')
                {
                    dup = strdup(trimmed);
                    free(line);
                    return (dup);
                }

                /* reset for next logical line */
                free(line);
                line = NULL;
                cap = 0;
                len = 0;

                /* continue scanning for next non-empty line */
                continue;
            }

            /* regular byte: append to dynamic line buffer */
            if (len + 1 >= cap)
            {
                size_t new_cap = (cap == 0) ? 128 : (cap * 2);
                char *tmp = realloc(line, new_cap);

                if (tmp == NULL)
                {
                    free(line);
                    return (NULL);
                }
                line = tmp;
                cap = new_cap;
            }
            line[len++] = rbuf[i];
        }

        /* consumed entire rbuf: reset positions to trigger refill */
        rpos = 0;
        rlen = 0;

        /* refill buffer */
        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            /* EOF or error: if we have a partial line without newline */
            if (line != NULL)
            {
                /* terminate, trim, and return if non-empty */
                if (len == cap)
                {
                    char *tmp2 = realloc(line, cap + 1);

                    if (tmp2 == NULL)
                    {
                        free(line);
                        return (NULL);
                    }
                    line = tmp2;
                    cap += 1;
                }
                line[len] = '\0';

                trimmed = trim_spaces(line);
                if (trimmed[0] != '\0')
                {
                    dup = strdup(trimmed);
                    free(line);
                    return (dup);
                }
                free(line);
            }
            /* nothing usable; finish */
            if (isatty(STDIN_FILENO))
                printf("\n");
            return (NULL);
        }
        rlen = nread;
        /* loop to process newly read bytes */
    }
}
