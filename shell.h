#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

void print_prompt(void);
char *read_command(void);
void execute_command(char *line, int interactive);
char *trim_spaces(char *str);
char *find_command(char *cmd);
char *get_env_path(void);
void print_not_found(const char *cmd);

#endif /* SHELL_H */
