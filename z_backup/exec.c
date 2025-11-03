#include "shell.h"

/**
 * run_command - run a command if it exists
 * @argv: array of command arguments
 * @progname: name of the shell program (for error messages)
 * Return: exit code (0 success, 127 not found, other error)
 */
int run_command(char **argv, const char *progname)
{
	pid_t pid;
	int status = 0;
	char *full = NULL;

	if (!argv || !argv[0])
		return (0);

	/* Find the executable path */
	if (find_executable(argv[0], &full) != 0)
	{
		/* Do NOT fork if not found */
		fprintf(stderr, "%s: 1: %s: not found\n",
			progname ? progname : "./hsh", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(full);
		return (1);
	}

	if (pid == 0)
	{
		/* Child process */
		execve(full, argv, environ);
		perror(argv[0]);
		free(full);
		_exit(126);
	}

	/* Parent process waits */
	free(full);
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		return (1);
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

