#define _GNU_SOURCE
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* safe strdup without calling strdup */
static char *_strdup(const char *s)
{
    size_t n;
    char *p;

    if (!s)
        return (NULL);
    n = strlen(s) + 1;
    p = (char *)malloc(n);
    if (!p)
        return (NULL);
    memcpy(p, s, n);
    return (p);
}

/* getenv is forbidden: parse environ manually */
static char *get_env(const char *name)
{
    extern char **environ;
    size_t len;
    char **p;

    len = strlen(name);
    for (p = environ; *p; p++)
    {
        if (strncmp(*p, name, len) == 0 && (*p)[len] == '=')
            return (*p + len + 1);
    }
    return (NULL);
}

int main(void)
{
    /* all declarations first (C89) */
    char *line, *tok, *path, *paths, *dir, *full, *args[64];
    size_t cap, need;
    ssize_t nread;
    int i, interactive, found;
    pid_t pid;
    extern char **environ;

    line = NULL;
    cap = 0;
    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        nread = getline(&line, &cap, stdin);
        if (nread == -1)
        {
            free(line);
            exit(0);
        }

        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';
        if (line[0] == '\0')
            continue;

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            exit(0);
        }

        /* tokenize */
        i = 0;
        tok = strtok(line, " \t");
        while (tok && i < 63)
        {
            args[i++] = tok;
            tok = strtok(NULL, " \t");
        }
        args[i] = NULL;
        if (!args[0])
            continue;

        /* absolute/relative path with '/' */
        if (strchr(args[0], '/'))
        {
            pid = fork();
            if (pid == 0)
            {
                execve(args[0], args, environ);
                perror("Error");
                _exit(1);
            }
            waitpid(pid, NULL, 0);
            continue;
        }

        /* search PATH */
        path = get_env("PATH");
        if (!path)
            path = (char *)"/bin:/usr/bin";

        paths = _strdup(path);
        if (!paths)
            continue;

        dir = strtok(paths, ":");
        found = 0;

        while (dir)
        {
            need = strlen(dir) + 1 + strlen(args[0]) + 1; /* dir + '/' + cmd + '\0' */
            full = (char *)malloc(need);
            if (!full)
                break;

            strcpy(full, dir);
            strcat(full, "/");
            strcat(full, args[0]);

            if (access(full, X_OK) == 0)
            {
                found = 1;
                pid = fork();
                if (pid == 0)
                {
                    execve(full, args, environ);
                    perror("Error");
                    _exit(1);
                }
                waitpid(pid, NULL, 0);
                free(full);
                break;
            }

            free(full);
            dir = strtok(NULL, ":");
        }

        free(paths);

        if (!found)
        {
            write(STDOUT_FILENO, "Command not found: ", 20);
            write(STDOUT_FILENO, args[0], strlen(args[0]));
            write(STDOUT_FILENO, "\n", 1);
        }
    }

    return (0);
}
