#include "shell.h"

/**
 * main - simple shell loop
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	char *line = NULL, *tok;
	size_t n = 0, cap = 0;
	ssize_t r;
	char **argv = NULL;
	int status = 0;

	(void)ac;
	(void)av;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		r = getline(&line, &n, stdin);
		if (r == -1)
			break;

		if (line[r - 1] == '\n')
			line[r - 1] = '\0';

		if (is_blank(line))
			continue;

		argv = grow_argv(argv, &cap, 1);
		if (!argv)
			break;

		tok = strtok(line, " ");
		while (tok)
		{
			argv = grow_argv(argv, &cap, cap + 1);
			if (!argv)
				break;
			argv[cap - 1] = strdup_safe(tok);
			tok = strtok(NULL, " ");
		}
		if (argv && argv[0])
			status = run_command(argv, av[0]);
		free_tokens(argv);
		argv = NULL;
	}
	free(line);
	return (status);
}

