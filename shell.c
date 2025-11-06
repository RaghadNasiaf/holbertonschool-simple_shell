#include "shell.h"

/**
* display_prompt - Displays shell prompt
*/
void display_prompt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);
}

/**
* read_input - Reads a line from stdin
*
* Return: Pointer to the input string, or NULL on EOF
*/
char *read_input(void)
{
char *input = NULL;
size_t size = 0;
ssize_t len;

len = getline(&input, &size, stdin);
if (len == -1)
{
free(input);
return (NULL);
}

if (len > 0 && input[len - 1] == '\n')
input[len - 1] = '\0';

return (input);
}

/**
* parse_input - Splits input into tokens
* @input: Input string to parse
*
* Return: Array of tokens, or NULL on failure
*/
char **parse_input(char *input)
{
char **args = malloc(MAX_ARGS * sizeof(char *));
char *token;
int i = 0;

if (!args)
return (NULL);

token = strtok(input, " \t");
while (token != NULL && i < MAX_ARGS - 1)
{
args[i] = strdup(token);
if (!args[i])
{
while (i > 0)
free(args[--i]);
free(args);
return (NULL);
}
i++;
token = strtok(NULL, " \t");
}
args[i] = NULL;

return (args);
}

/**
* free_args - Frees the args array
* @args: Array to free
*/
void free_args(char **args)
{
int i = 0;

if (!args)
return;

while (args[i])
{
free(args[i]);
i++;
}
free(args);
}

/**
* execute_command - Executes a command with arguments
* @args: Command and arguments
*
* Return: Exit status of the command
*/
int execute_command(char **args)
{
pid_t pid;
int status;
struct stat st;

if (!args || !args[0])
return (0);

/* Check if command exists */
if (stat(args[0], &st) == -1)
{
write(STDERR_FILENO, "./hsh: 1: ", 10);
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
return (127);
}

pid = fork();
if (pid == 0)
{
/* Child process */
if (execve(args[0], args, environ) == -1)
{
write(STDERR_FILENO, "./hsh: 1: ", 10);
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
exit(127);
}
}
else if (pid > 0)
{
/* Parent process - wait for child to complete */
wait(&status);

if (WIFEXITED(status))
return (WEXITSTATUS(status));
}
else
{
perror("fork");
return (1);
}

return (0);
}
