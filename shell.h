#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

/* utils */
char *strdup_safe(const char *s);
char **tokenize(char *line, int *argc);
void free_tokens(char **argv);
char *dup_env_value(const char *name);
char *find_in_path(const char *cmd);

/* core */
int run_command(char **argv);

#endif /* SHELL_H */

