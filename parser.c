#include "shell.h"

/**
 * trim_spaces - elimina espacios y tabs iniciales y finales
 * @str: cadena de entrada
 *
 * Return: puntero a cadena recortada
 */
char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '\0')
        return (str);

    end = str + _strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
    {
        *end = '\0';
        end--;
    }
    return (str);
}

/**
 * tokenize_line - separa una linea en tokens por espacio o tab
 * @line: cadena de entrada (se modifica in place)
 *
 * Return: arreglo de tokens terminado en NULL (malloc'd)
 */
char **tokenize_line(char *line)
{
    char **argv;
    int cap = 64, argc = 0;
    char *p = line, *start;

    argv = malloc(sizeof(char *) * cap);
    if (!argv)
        return (NULL);

    while (*p)
    {
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '\0')
            break;

        start = p;
        while (*p && *p != ' ' && *p != '\t')
            p++;
        if (*p)
        {
            *p = '\0';
            p++;
        }

        argv[argc++] = start;
        if (argc + 1 >= cap)
            break;
    }
    argv[argc] = NULL;
    return (argv);
}
