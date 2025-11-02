/**
 * @file simple_shell.c
 * @author Raghad Naseef
 * @email naseefraghad@gmail.com
 * @brief Simple shell (task 0.2): handle command lines with arguments.
 *
 * Requirements covered:
 * - Display a prompt and wait for user input.
 * - Execute commands given with a full path (e.g., /bin/ls -l /tmp).
 * - No handling of PATH yet (that is for later tasks).
 * - No separators, no pipes, no redirections.
 */

#include "main.h"

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *argv[128];
	pid_t pid;

	while (1)
	{
		/* print prompt */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* read input line */
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* EOF (Ctrl+D) or error */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* strip trailing newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* tokenize into argv */
		size_t i = 0;
		char *tok = strtok(line, " \t");
		while (tok && i + 1 < sizeof(argv) / sizeof(argv[0]))
		{
			argv[i++] = tok;
			tok = strtok(NULL, " \t");
		}
		argv[i] = NULL;

		/* empty line => show prompt again */
		if (argv[0] == NULL)
			continue;

		/* fork and exec */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		if (pid == 0)
		{
			/* child: exec with provided absolute/relative path */
			execve(argv[0], argv, environ);
			perror("Error"); /* only reached if execve fails */
			_exit(127);
		}
		else
		{
			/* parent: wait for child */
			int status;
			if (waitpid(pid, &status, 0) == -1)
				perror("waitpid");
			(void)status;
		}
	}

	free(line);
	return 0;
}
