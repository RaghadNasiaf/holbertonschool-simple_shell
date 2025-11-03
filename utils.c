#include "shell.h"

/**
 * strdup_safe - duplicate a string safely
 * @s: string to duplicate
 * Return: pointer to new string, or NULL on failure
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

	memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

/**
 * is_blank - check if string is empty or only spaces/tabs/newlines
 * @s: input string
 * Return: 1 if blank, 0 otherwise
 */
int is_blank(const char *s)
{
	size_t i;

	if (!s)
		return (1);

	for (i = 0; s[i]; i++)
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			return (0);
	}
	return (1);
}

/**
 * free_tokens - free array of strings
 * @argv: array of tokens (NULL-terminated or sized)
 *
 * Frees every non-NULL string, then frees the array.
 * Resets internal capacity used by grow_argv().
 */
void free_tokens(char **argv)
{
	size_t i;

	if (!argv)
		return;

	for (i = 0; argv[i]; i++)
		free(argv[i]);

	free(argv);

	/* reset internal capacity (paired with grow_argv) */
	{
		extern size_t __argv_capacity_internal;
		__argv_capacity_internal = 0;
	}
}

/* ---- internal capacity used by grow_argv (single argv at a time) ---- */
size_t __argv_capacity_internal = 0;

/**
 * grow_argv - ensure argv has room for @needed elements (logical length)
 * @argv: current array (may be NULL)
 * @cap:  pointer to current logical length (we treat it as length, not cap)
 * @needed: new logical length required (e.g., old length + 1)
 *
 * Behavior:
 * - Allocates/resizes the underlying array capacity (power-of-two growth)
 *   without using realloc.
 * - Updates *@cap to @needed.
 * - Leaves all unused slots initialized to NULL.
 *
 * Return: pointer to (possibly new) array, or NULL on failure.
 */
char **grow_argv(char **argv, size_t *cap, size_t needed)
{
	size_t newcap, i;
	char **tmp;

	if (!cap || needed == 0)
		return (argv);

	/* grow physical capacity if needed */
	if (__argv_capacity_internal < needed)
	{
		newcap = (__argv_capacity_internal == 0) ? 8 : __argv_capacity_internal;
		while (newcap < needed)
			newcap *= 2;

		tmp = malloc(newcap * sizeof(char *));
		if (!tmp)
			return (NULL);

		/* copy old pointers (up to old logical length) */
		if (argv && *cap)
		{
			for (i = 0; i < *cap; i++)
				tmp[i] = argv[i];
			free(argv);
		}
		else
		{
			i = 0;
		}

		/* NULL-out the remaining slots */
		for (; i < newcap; i++)
			tmp[i] = NULL;

		argv = tmp;
		__argv_capacity_internal = newcap;
	}

	/* update logical length (caller uses indexes [0..*cap-1]) */
	*cap = needed;

	/* keep array NULL-terminated as much as possible */
	if (*cap < __argv_capacity_internal)
		argv[*cap] = NULL;

	return (argv);
}

