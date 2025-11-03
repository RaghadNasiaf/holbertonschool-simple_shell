#include "shell.h"

/* duplicate string safely */
char *strdup_safe(const char *s)
{
    size_t len;
    char *dup;

    if (!s) return NULL;
    len = strlen(s);
    dup = malloc(len + 1);
    if (!dup) return NULL;
    memcpy(dup, s, len + 1);
    return dup;
}

/* check empty or only spaces/tabs/newlines */
int is_blank(const char *s)
{
    if (!s) return 1;
    while (*s)
    {
        if (*s != ' ' && *s != '\t' && *s != '\n')
            return 0;
        s++;
    }
    return 1;
}

/* free argv list */
void free_tokens(char **argv)
{
    size_t i;

    if (!argv) return;
    for (i = 0; argv[i]; i++)
        free(argv[i]);
    free(argv);
}

/* grow argv capacity without realloc (realloc ممنوع) */
char **grow_argv(char **argv, size_t *cap, size_t needed)
{
    size_t oldcap = cap && *cap ? *cap : 0;
    size_t newcap = oldcap ? oldcap : 8;
    char **tmp;

    while (newcap <= needed)
        newcap *= 2;

    tmp = malloc(newcap * sizeof(*tmp));
    if (!tmp) return NULL;

    if (argv && oldcap)
    {
        memcpy(tmp, argv, oldcap * sizeof(*tmp));
        free(argv);
    }
    *cap = newcap;
    return tmp;
}

