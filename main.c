#include "shell.h"

/**
 * main - Entry point of the shell
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
    char *line;
    int interactive;

    (void)argc;
    (void)argv;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            print_prompt();

        line = read_command();
        if (line == NULL)
            break;

        execute_command(line, interactive);
        free(line);
    }
    return (0);
}
