#include "shell.h"

char *strdup_safe(const char *s)
{
	size_t n;
	char *p;

	if (!s)
		return (NULL);
	n = strlen(s) + 1;
	p = malloc(n);
	if (!p)
		return (NULL);
	memcpy(p, s, n);
	return (p);
}

char **tokenize(char *line, int *argc)
{
	char *tok, **argv = NULL;
	size_t cap = 0;
	const char *delim = " \t\r\n";

	*argc = 0;
	tok = strtok(line, delim);
	while (tok)
	{
		if (*argc + 1 >= (int)cap)
		{
			size_t new_cap = cap ? cap * 2 : 8;
			char **tmp = realloc(argv, new_cap * sizeof(*argv));
			if (!tmp)
			{
				free_tokens(argv);
				return (NULL);
			}
			argv = tmp;
			cap = new_cap;
		}
		argv[*argc] = strdup_safe(tok);
		if (!argv[*argc])
		{
			free_tokens(argv);
			return (NULL);
		}
		(*argc)++;
		tok = strtok(NULL, delim);
	}
	if (!argv)
	{
		argv = malloc(2 * sizeof(*argv));
		if (!argv)
			return (NULL);
	}
	argv[*argc] = NULL;
	return (argv);
}

void free_tokens(char **argv)
{
	int i;

	if (!argv)
		return;
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

static int is_executable(const char *path)
{
	struct stat st;

	return (path && stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char *find_in_path(const char *cmd)
{
	char *path, *copy, *dir, *full;
	size_t len;

	if (!cmd || strchr(cmd, '/'))
		return (is_executable(cmd) ? strdup_safe(cmd) : NULL);

	path = getenv("PATH");
	if (!path)
		return (NULL);

	copy = strdup_safe(path);
	if (!copy)
		return (NULL);

	dir = strtok(copy, ":");
	while (dir)
	{
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		full = malloc(len);
		if (!full)
		{
			free(copy);
			return (NULL);
		}
		snprintf(full, len, "%s/%s", dir, cmd);
		if (is_executable(full))
		{
			free(copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}
	free(copy);
	return (NULL);
}
