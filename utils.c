#include "shell.h"

/* safe strdup using malloc */
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

/* simple tokenizer: splits by spaces/tabs/newlines */
char **tokenize(char *line, int *argc)
{
	char *tok, **argv = NULL;
	size_t cap = 0, i;
	const char *delim = " \t\r\n";

	if (!argc)
		return (NULL);
	*argc = 0;

	tok = strtok(line, delim);
	while (tok)
	{
		if ((size_t)(*argc + 1) >= cap)
		{
			size_t new_cap = cap ? cap * 2 : 8;
			char **tmp = malloc(new_cap * sizeof(*tmp));
			if (!tmp)
			{
				free_tokens(argv);
				return (NULL);
			}
			for (i = 0; i < (size_t)*argc; i++)
				tmp[i] = argv ? argv[i] : NULL;
			free(argv);
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
		*argc = 0;
	}
	argv[*argc] = NULL;
	return (argv);
}

/* free array of tokens */
void free_tokens(char **argv)
{
	int i;

	if (!argv)
		return;
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

/* duplicate value of an env var by scanning environ (no getenv) */
char *dup_env_value(const char *name)
{
	size_t namelen;
	int i;

	if (!name)
		return (NULL);
	namelen = strlen(name);

	for (i = 0; environ && environ[i]; i++)
	{
		if (strncmp(environ[i], name, namelen) == 0 && environ[i][namelen] == '=')
			return (strdup_safe(environ[i] + namelen + 1));
	}
	return (NULL);
}

/* check if path is executable */
static int is_executable(const char *path)
{
	struct stat st;

	if (!path)
		return (0);
	if (stat(path, &st) == -1)
		return (0);
	return (access(path, X_OK) == 0);
}

/* search cmd in PATH, returns malloc'ed full path or NULL */
char *find_in_path(const char *cmd)
{
	char *path_copy, *dir, *full;
	size_t len;

	if (!cmd || !*cmd)
		return (NULL);

	if (strchr(cmd, '/'))
		return (is_executable(cmd) ? strdup_safe(cmd) : NULL);

	path_copy = dup_env_value("PATH");
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		full = malloc(len);
		if (!full)
		{
			free(path_copy);
			return (NULL);
		}
		snprintf(full, len, "%s/%s", dir, cmd);
		if (is_executable(full))
		{
			free(path_copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

