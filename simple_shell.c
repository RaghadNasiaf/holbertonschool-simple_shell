#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *args[64];
    pid_t pid;
    int status;
    size_t i;
    char *tok;

    while (1)
    {
        /* show prompt only if in interactive mode */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        /* read user input */
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* remove trailing newline */
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* tokenize input line */
        i = 0;
        tok = strtok(line, " \t");
        while (tok != NULL && i < 63)
        {
            args[i++] = tok;
            tok = strtok(NULL, " \t");
        }
        args[i] = NULL;

        /* if empty command, continue */
        if (args[0] == NULL)
            continue;

        /* fork and execute */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        if (pid == 0)
        {
            execve(args[0], args, NULL);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return (0);
}

