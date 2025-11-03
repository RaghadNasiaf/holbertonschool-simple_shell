#include "shell.h"

/**
 * tokenize - split input line into tokens
 * @line: input string
 * Return: array of tokens or NULL
 */
char **tokenize(char *line)
{
	char **argv = NULL, *tok, *save;
	size_t cap = 0, count = 0;

	for (tok = strtok_r(line, " \t\n", &save); tok; tok = strtok_r(NULL, " \t\n", &save))
	{
		if (count + 2 > cap)
		{
			size_t newcap = cap ? cap * 2 : 8;
			char **tmp = realloc(argv, newcap * sizeof(char *));
			if (!tmp)
			{
				free_tokens(argv);
				return (NULL);
			}
			argv = tmp;
			cap = newcap;
		}
		argv[count] = strdup_safe(tok);
		if (!argv[count])
		{
			free_tokens(argv);
			return (NULL);
		}
		count++;
	}
	if (!argv)
	{
		argv = malloc(sizeof(char *));
		if (!argv)
			return (NULL);
		argv[0] = NULL;
		return (argv);
	}
	argv[count] = NULL;
	return (argv);
}

/**
 * main - simple shell main loop
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	int interactive = isatty(STDIN_FILENO);
	(void)argc;

	while (1)
	{
		char **av;
		int code;

		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);

		r = getline(&line, &n, stdin);
		if (r == -1) /* EOF or error */
			break;

		if (is_blank(line))
			continue;

		av = tokenize(line);
		if (!av || !av[0])
		{
			free_tokens(av);
			continue;
		}

		if (strcmp(av[0], "exit") == 0)
		{
			free_tokens(av);
			break;
		}

		code = run_command(av, argv[0] ? argv[0] : "./hsh");
		free_tokens(av);

		if (!interactive)
			(void)code;
	}

	free(line);
	return (0);
}

