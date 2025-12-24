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
#define TOK_SIZE  1024

/**
 * read_command - reads one logical line using read(), robust to huge inputs
 * Behavior:
 * - Processes input byte-by-byte with an internal buffer and preserves leftovers.
 * - Skips any amount of leading spaces/tabs.
 * - Extracts only the first non-space token of the line (the command),
 *   ignoring trailing spaces or extra characters until newline.
 * - Handles arbitrarily large lines without realloc by only buffering the token.
 * - Returns one command per call (trimmed), or NULL on EOF/error.
 * - Uses only allowed functions (no getline/fgets/realloc).
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;  /* bytes currently in rbuf */
    static ssize_t rpos = 0;  /* current read position in rbuf */

    char token[TOK_SIZE];
    ssize_t tlen = 0;
    int in_token = 0;         /* 0: skipping leading spaces, 1: collecting token */
    ssize_t nread, i;
    char *dup;

    for (;;)
    {
        /* Consume buffered bytes */
        for (i = rpos; i < rlen; i++)
        {
            char c = rbuf[i];

            if (c == '\n')
            {
                /* End of logical line: if we collected a token, return it */
                if (in_token && tlen > 0)
                {
                    token[tlen] = '\0';
                    rpos = i + 1;
                    dup = strdup(token);
                    return (dup);
                }
                /* No token in this line: reset and look for next line */
                in_token = 0;
                tlen = 0;
                rpos = i + 1;
                continue;
            }

            /* Handle spaces/tabs */
            if (c == ' ' || c == '\t')
            {
                if (in_token)
                {
                    /* We already have the command token; ignore the rest of the line until newline */
                    continue;
                }
                /* Still skipping leading spaces */
                continue;
            }

            /* Non-space character */
            if (!in_token)
                in_token = 1;

            /* Append to token buffer (truncate if exceeds TOK_SIZE - 1) */
            if (tlen < (ssize_t)(TOK_SIZE - 1))
                token[tlen++] = c;
            /* else: ignore extra chars in token; typical commands fit in TOK_SIZE */
        }

        /* Buffer exhausted: reset to trigger refill */
        rpos = 0;
        rlen = 0;

        /* Read more data */
        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            /* EOF or error: if we have a partial token, return it */
            if (in_token && tlen > 0)
            {
                token[tlen] = '\0';
                dup = strdup(token);
                return (dup);
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
