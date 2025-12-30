#include "shell.h"

/**
 * main - entry point of the shell
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: exit status of last executed command
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    return (run_shell());
}
