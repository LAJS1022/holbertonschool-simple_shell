#include "shell.h"

/**
 * print_env - imprime las variables de entorno actuales
 */
void print_env(void)
{
    int i;

    for (i = 0; environ && environ[i]; i++)
    {
        write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
        write(STDOUT_FILENO, "\n", 1);
    }
}

/**
 * execute - ejecuta un comando con argumentos
 * @args: vector de argumentos terminado en NULL
 * @status: puntero al estado para actualizar
 */
void execute(char **args, int *status)
{
    char *path;
    pid_t pid;

    if (!args || !args[0])
        return;

    path = resolve_command(args[0]);
    if (!path)
        return;

    pid = fork();
    if (pid == -1)
    {
        *status = 1;
        free(path);
        return;
    }
    if (pid == 0)
    {
        execve(path, args, environ);
        _exit(126);
    }
    if (waitpid(pid, status, 0) == -1)
        *status = 1;
    else if (WIFEXITED(*status))
        *status = WEXITSTATUS(*status);
    else
        *status = 1;

    free(path);
}
