#include "shell.h"

/**
* display_prompt - Displays shell prompt
*/
void display_prompt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "($) ", 4);
}

/**
* read_input - Reads a line from stdin
* Return: Pointer to the input string
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

/* Remove newline */
if (input[len - 1] == '\n')
input[len - 1] = '\0';

return (input);
}

/**
* parse_input - Splits input into tokens (single word for now)
* @input: Input string to parse
* Return: Array of tokens
*/
char **parse_input(char *input)
{
char **args = malloc(2 * sizeof(char *));

if (!args)
return (NULL);

args[0] = malloc(strlen(input) + 1);
if (!args[0])
{
free(args);
return (NULL);
}

strcpy(args[0], input);
args[1] = NULL;

return (args);
}

/**
* execute_command - Executes a command
* @args: Command and arguments
* Return: 1 to continue, 0 to exit
*/
int execute_command(char **args)
{
pid_t pid;
int status;
struct stat st;

/* Check if file exists */
if (stat(args[0], &st) == -1)
{
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
return (1);
}

pid = fork();
if (pid == 0)
{
/* Child process */
if (execve(args[0], args, environ) == -1)
{
perror("Error");
exit(1);
}
}
else if (pid > 0)
{
/* Parent process */
wait(&status);
}
else
{
/* Fork failed */
perror("Fork failed");
}

return (1);
}
