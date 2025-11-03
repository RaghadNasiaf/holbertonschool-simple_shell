#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/* utils.c */
char *strdup_safe(const char *s);
int   is_blank(const char *s);
void  free_tokens(char **argv);
char **grow_argv(char **argv, size_t *cap, size_t needed);

/* path.c */
char *get_env_value(const char *name);
int   find_executable(const char *cmd, char **fullpath);

/* exec.c */
int   run_command(char **argv, const char *progname);

#endif /* SHELL_H */

