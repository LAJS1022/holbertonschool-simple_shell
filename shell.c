#include "shell.h"

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
printf("#cisfun$ ");
}

/**
 * read_command - reads a line from stdin
 *
 * Return: pointer to the line, or NULL on EOF
 */
char *read_command(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;

read = getline(&line, &len, stdin);
if (read == -1)
{
free(line);
printf("\n");
return (NULL);
}

line[strcspn(line, "\n")] = '\0';
if (line[0] == '\0')
{
free(line);
return (read_command());
}

return (line);
}

/**
 * execute_command - forks and executes a command (no PATH, no built-ins)
 * @line: command string
 */
void execute_command(char *line)
{
pid_t pid;
char *argv[2];

argv[0] = line;
argv[1] = NULL;

pid = fork();
if (pid == -1)
{
perror("fork");
return;
}
if (pid == 0)
{
if (execve(argv[0], argv, environ) == -1)
{
perror(argv[0]);
exit(1);
}
}
else
{
wait(NULL);
}
}

/**
 * main - entry point for the shell
 *
 * Return: Always 0
 */
int main(void)
{
char *line;

while (1)
{
if (isatty(STDIN_FILENO))
print_prompt();

line = read_command();
if (line == NULL)
break;

execute_command(line);
free(line);
}
return (0);
}
