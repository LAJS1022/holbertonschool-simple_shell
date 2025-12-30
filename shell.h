#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;
extern int last_status;

void run_shell(void);
void tokenize(char *line, char **args);
char *resolve_path(char *cmd);
int exec_cmd(char *cmd, char **argv);

#endif /* SHELL_H */
