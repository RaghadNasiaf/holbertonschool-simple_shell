#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * get_env_value - retrieves an environment variable directly from environ
 * @name: variable name (e.g. "PATH")
 * Return: pointer to value or NULL if not found
 */
char *get_env_value(const char *name)
{
	size_t n = strlen(name);
	char **env;

	for (env = environ; *env; env++)
	{
		if (strncmp(*env, name, n) == 0 && (*env)[n] == '=')
			return (*env + n + 1);
	}
	return (NULL);
}

/**
 * build_command_path - builds the full path for a command using PATH
 * @cmd: command name
 * Return: pointer to full path (malloc’d), or NULL if not found
 */
char *build_command_path(const char *cmd)
{
	char *path_value, *path_copy, *dir, *full_path;
	size_t len;

	if (cmd == NULL)
		return (NULL);

	path_value = get_env_value("PATH");
	if (!path_value)
		return (NULL);

	path_copy = strdup(path_value);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

