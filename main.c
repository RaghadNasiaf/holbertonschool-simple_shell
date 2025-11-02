#include "shell.h"

int run_command(char **argv)
{
	pid_t pid;
	int status;
	char *path;

	if (!argv || !argv[0])
		return (0);

	path = find_in_path(argv[0]);
	if (!path)
	{
		dprintf(STDERR_FILENO, "%s: command not found\n", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(path);
		return (1);
	}
	if (pid == 0)
	{
		execve(path, argv, environ);
		perror("execve");
		_exit(126);
	}
	free(path);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int main(void)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	int argc;
	char **argv;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		r = getline(&line, &n, stdin);
		if (r == -1)
			break; /* Ctrl+D or EOF */

		if (r > 1)
		{
			argv = tokenize(line, &argc);
			if (!argv)
			{
				perror("tokenize");
				continue;
			}
			if (argc > 0)
			{
				if (strcmp(argv[0], "exit") == 0)
				{
					free_tokens(argv);
					break;
				}
				run_command(argv);
			}
			free_tokens(argv);
		}
	}
	free(line);
	return (0);
}
