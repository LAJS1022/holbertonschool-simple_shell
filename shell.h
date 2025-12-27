#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char **parse_line(char *line);
void execute(char **argv);
char *find_command(const char *cmd);
char *read_line(void);
void free_args(char **argv);
int builtin_exit(char **argv);
int builtin_env(char **argv);
size_t _strlen(const char *s);
char *_strdup(const char *s);
int _strcmp(const char *s1, const char *s2);

#endif /* SHELL_H */
