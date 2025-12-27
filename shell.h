#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>

extern char **environ;

char *trim_spaces(char *str);
char **tokenize_line(char *line);
char *get_env_value(const char *name);
char *resolve_command(const char *cmd);
void print_env(void);
void execute(char **args, int *status);
size_t _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
char *_strchr(const char *s, int c);
void *_memcpy(void *dest, const void *src, size_t n);
char *_strcpy(char *dest, const char *src);

#endif /* SHELL_H */
