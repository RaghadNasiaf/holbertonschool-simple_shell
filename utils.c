#include "shell.h"

/**
 * strdup_safe - duplicate a string safely
 * @s: string to duplicate
 * Return: pointer to new string or NULL
 */
char *strdup_safe(const char *s)
{
	size_t len;
	char *dup;

	if (!s)
		return (NULL);
	len = strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	memcpy(dup, s, len + 1);
	return (dup);
}

/**
 * is_blank - check if string is empty or only spaces/tabs/newlines
 * @s: input string
 * Return: 1 if blank, 0 otherwise
 */
int is_blank(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\n')
			return (0);
		s++;
	}
	return (1);
}

/**
 * free_tokens - free array of strings
 * @argv: array of tokens
 */
void free_tokens(char **argv)
{
	size_t i;

	if (!argv)
		return;
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

