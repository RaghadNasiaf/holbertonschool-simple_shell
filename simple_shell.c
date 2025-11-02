/* simple_shell.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char **argv)
{
	char *line, *path, *path_copy, *token, *cmd_path;
	char *args[64];
	size_t len;
	ssize_t nread;
	pid_t pid;
	int status, i;
	(void)argc;
	(void)argv;

	line = NULL;
	len = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* tokenize input */
		args[0] = strtok(line, " \t");
		i = 0;
		while (args[i] != NULL && i < 63)
		{
			i++;
			args[i] = strtok(NULL, " \t");
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;
		if (strcmp(args[0], "exit") == 0)
			break;

		/* find command in PATH */
		cmd_path = NULL;
		path = getenv("PATH");
		if (path != NULL)
		{
			path_copy = strdup(path);
			if (path_copy == NULL)
				continue;

			token = strtok(path_copy, ":");
			while (token != NULL)
			{
				char temp[512];

				/* build candidate: token + "/" + cmd */
				snprintf(temp, sizeof(temp), "%s/%s", token, args[0]);
				if (access(temp, X_OK) == 0)
				{
					cmd_path = strdup(temp);
					break;
				}
				token = strtok(NULL, ":");
			}
			free(path_copy);
		}

		/* if not found in PATH, try as given (absolute/relative) */
		if (cmd_path == NULL && access(args[0], X_OK) == 0)
			cmd_path = strdup(args[0]);

		if (cmd_path == NULL)
		{
			dprintf(STDERR_FILENO, "%s: command not found\n", args[0]);
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(cmd_path);
			continue;
		}
		if (pid == 0)
		{
			execve(cmd_path, args, environ);
			perror("execve");
			_exit(127);
		}
		waitpid(pid, &status, 0);
		free(cmd_path);
	}

	free(line);
	return 0;
}

