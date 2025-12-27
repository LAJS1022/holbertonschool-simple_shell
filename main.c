#include "shell.h"

/**
 * main - simple shell con built-ins exit y env
 * @argc: numero de argumentos
 * @argv: vector de argumentos
 *
 * Return: estado de salida
 */
int main(int argc, char **argv)
{
    char *line = NULL, *cmd;
    size_t n = 0;
    ssize_t read;
    char **args;
    int status = 0;

    (void)argc;
    (void)argv;

    for (;;)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        read = getline(&line, &n, stdin);
        if (read == -1)
        {
            free(line);
            return (status);
        }

        cmd = strtok(line, "\n");
        while (cmd)
        {
            cmd = trim_spaces(cmd);
            if (*cmd == '\0')
            {
                cmd = strtok(NULL, "\n");
                continue;
            }

            args = tokenize_line(cmd);
            if (!args || !args[0] || args[0][0] == '\0')
            {
                if (args)
                    free(args);
                cmd = strtok(NULL, "\n");
                continue;
            }

            if (_strcmp(args[0], "exit") == 0)
            {
                free(args);
                free(line);
                exit(status);
            }

            if (_strcmp(args[0], "env") == 0)
            {
                print_env();
                free(args);
                cmd = strtok(NULL, "\n");
                continue;
            }

            if (!resolve_command(args[0]))
            {
                dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", args[0]);
                status = 127;
                free(args);
                cmd = strtok(NULL, "\n");
                continue;
            }

            execute(args, &status);
            free(args);

            cmd = strtok(NULL, "\n");
        }
    }

    free(line);
    return (status);
}
