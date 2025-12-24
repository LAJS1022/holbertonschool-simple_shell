#include "shell.h"

#define RBUF_SIZE 4096
#define LINE_SIZE 4096

/**
 * read_command - reads one logical trimmed line using read()
 * Return: newly allocated command string or NULL on EOF/error
 */
char *read_command(void)
{
    static char rbuf[RBUF_SIZE];
    static ssize_t rlen = 0;
    static ssize_t rpos = 0;

    char line[LINE_SIZE];
    ssize_t linelen = 0;
    ssize_t last_nons = -1;
    int started = 0;
    int discarding = 0;
    ssize_t i, nread;
    char *dup;

    for (;;)
    {
        for (i = rpos; i < rlen; i++)
        {
            char c = rbuf[i];

            if (c == '\n')
            {
                if (!discarding && last_nons >= 0)
                {
                    line[last_nons + 1] = '\0';
                    dup = strdup(line);
                    rpos = i + 1;
                    return (dup);
                }
                linelen = 0;
                last_nons = -1;
                started = 0;
                discarding = 0;
                rpos = i + 1;
                continue;
            }

            if (discarding)
                continue;

            if (!started)
            {
                if (c == ' ' || c == '\t')
                    continue;
                started = 1;
            }

            if (linelen < (ssize_t)(LINE_SIZE - 1))
            {
                line[linelen++] = c;
                if (c != ' ' && c != '\t')
                    last_nons = linelen - 1;
            }
            else
            {
                discarding = 1;
            }
        }

        rpos = 0;
        rlen = 0;

        nread = read(STDIN_FILENO, rbuf, RBUF_SIZE);
        if (nread <= 0)
        {
            if (!discarding && last_nons >= 0)
            {
                line[last_nons + 1] = '\0';
                dup = strdup(line);
                return (dup);
            }
            if (isatty(STDIN_FILENO))
                printf("\n");
            return (NULL);
        }
        rlen = nread;
    }
}
