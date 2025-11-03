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

/* utils.c */
char *strdup_safe(const char *s);
int is_blank(const char *s);
void free_tokens(char **argv);

/* path.c */
char *get_env_value(const char *name);
int find_executable(const char *cmd, char **fullpath);

/* exec.c */
int run_command(char **argv, const char *progname);

/* shell.c */
char **tokenize(char *line);
int main(int argc, char **argv);

#endif /* SHELL_H */

