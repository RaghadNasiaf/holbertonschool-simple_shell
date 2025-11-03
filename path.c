#include "shell.h"

/**
 * get_env_value - returns value of an environment variable by scanning environ
 * @name: var name (e.g. "PATH")
 * Return: pointer into environ string (do not free) or NULL
 */
char *get_env_value(const char *name)
{
	size_t n;
	char **p;

	if (!name)
		return (NULL);
	n = strlen(name);
	for (p = environ; *p; p++)
	{
        /* NAME=VALUE */
		if (strncmp(*p, name, n) == 0 && (*p)[n] == '=')
			return (*p + n + 1);
	}
	return (NULL);
}

/**
 * find_executable - locate executable using PATH or direct path
 * @cmd: command name
 * @fullpath: out pointer to malloc'd full path
 * Return: 0 on success, -1 not found/error
 */
int find_executable(const char *cmd, char **fullpath)
{
	char *path, *buf, *save, *tok;
	size_t dlen, clen;

	if (!cmd || !*cmd || !fullpath)
		return (-1);

	/* إذا فيه '/' نتعامل معه كمسار مباشر */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			*fullpath = strdup_safe(cmd);
			return (*fullpath ? 0 : -1);
		}
		return (-1);
	}

	path = get_env_value("PATH");
	if (!path)
		return (-1);

	buf = strdup_safe(path);
	if (!buf)
		return (-1);

	clen = strlen(cmd);

	for (tok = strtok_r(buf, ":", &save); tok; tok = strtok_r(NULL, ":", &save))
	{
		char *fp;
		dlen = strlen(tok);

		/* alloc: dir + '/' + cmd + '\0' */
		fp = malloc(dlen + 1 + clen + 1);
		if (!fp)
		{
			free(buf);
			return (-1);
		}

		memcpy(fp, tok, dlen);
		fp[dlen] = '/';
		memcpy(fp + dlen + 1, cmd, clen);
		fp[dlen + 1 + clen] = '\0';

		if (access(fp, X_OK) == 0)
		{
			free(buf);
			*fullpath = fp;
			return (0);
		}
		free(fp);
	}
	free(buf);
	return (-1);
}

