#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * main - simple shell 0.3: search commands in PATH
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL, *token, *path, *path_copy, *dir, *full_path;
    size_t len = 0;
    ssize_t nread;
    char *args[64];
    int i;
    int found;

    while (1)
    {
        printf("#cisfun$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            free(line);
            exit(0);
        }

        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            exit(0);
        }

        /* Split command and arguments */
        i = 0;
        token = strtok(line, " ");
        while (token && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        if (!args[0])
            continue;

        /* If command contains '/', try directly */
        if (strchr(args[0], '/'))
        {
            if (fork() == 0)
            {
                execve(args[0], args, environ);
                perror("Error");
                exit(1);
            }
            else
            {
                wait(NULL);
            }
            continue;
        }

        /* Search command in PATH */
        path = getenv("PATH");
        if (!path)
            path = "/bin:/usr/bin";
        path_copy = strdup(path);
        dir = strtok(path_copy, ":");
        found = 0;

        while (dir)
        {
            full_path = malloc(strlen(dir) + strlen(args[0]) + 2);
            sprintf(full_path, "%s/%s", dir, args[0]);

            if (access(full_path, X_OK) == 0)
            {
                found = 1;
                if (fork() == 0)
                {
                    execve(full_path, args, environ);
                    perror("Error");
                    exit(1);
                }
                else
                {
                    wait(NULL);
                    free(full_path);
                    break;
                }
            }

            free(full_path);
            dir = strtok(NULL, ":");
        }

        free(path_copy);
        if (!found)
            printf("Command not found: %s\n", args[0]);
    }

    return (0);
}

