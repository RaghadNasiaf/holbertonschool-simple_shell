#include "shell.h"

/**
* display_prompt - Displays shell prompt
*/
void display_prompt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, ":) ", 3);
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

/* Remove newline character */
if (len > 0 && input[len - 1] == '\n')
input[len - 1] = '\0';

return (input);
}

/**
* parse_input - Splits input into tokens (handles arguments)
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
args[i] = malloc(strlen(token) + 1);
if (!args[i])
{
/* Free previously allocated memory */
while (i > 0)
free(args[--i]);
free(args);
return (NULL);
}
strcpy(args[i], token);
i++;
token = strtok(NULL, " \t");
}
args[i] = NULL;

return (args);
}

/**
* command_exists - Checks if a command exists and is executable
* @command: Command to check
*
* Return: 1 if exists and executable, 0 otherwise
*/
int command_exists(char *command)
{
struct stat st;
char *full_path;

/* If command contains '/', check directly */
if (strchr(command, '/'))
{
if (stat(command, &st) == 0 && access(command, X_OK) == 0)
return (1);
return (0);
}

/* Otherwise, search in PATH */
full_path = find_command(command);
if (full_path)
{
free(full_path);
return (1);
}

return (0);
}

/**
* find_command - Finds command in PATH
* @command: Command to find
*
* Return: Full path to command, or NULL if not found
*/
char *find_command(char *command)
{
char *path, *path_copy, *dir, *full_path;
struct stat st;

if (!command)
return (NULL);

/* If command contains '/', return it as is */
if (strchr(command, '/'))
{
if (stat(command, &st) == 0 && access(command, X_OK) == 0)
return (strdup(command));
return (NULL);
}

/* Get PATH environment variable */
path = getenv("PATH");
if (!path)
return (NULL);

path_copy = strdup(path);
if (!path_copy)
return (NULL);

dir = strtok(path_copy, ":");
while (dir)
{
full_path = malloc(strlen(dir) + strlen(command) + 2);
if (!full_path)
{
free(path_copy);
return (NULL);
}

sprintf(full_path, "%s/%s", dir, command);

if (stat(full_path, &st) == 0 && access(full_path, X_OK) == 0)
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

/**
* execute_command - Executes a command with arguments
* @args: Command and arguments
*
* Return: 1 to continue, 0 to exit
*/
int execute_command(char **args)
{
pid_t pid;
int status;
char *full_path;

if (!args || !args[0])
return (1);

/* Check if command exists before forking */
if (!command_exists(args[0]))
{
/* Match the exact error format from the example */
write(STDERR_FILENO, "./shell_0.3: 1: ", 16);
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
return (1);
}

/* Find the full path to the command */
full_path = find_command(args[0]);
if (!full_path)
{
write(STDERR_FILENO, "./shell_0.3: 1: ", 16);
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
return (1);
}

pid = fork();
if (pid == 0)
{
/* Child process */
if (execve(full_path, args, environ) == -1)
{
write(STDERR_FILENO, "./shell_0.3: 1: ", 16);
write(STDERR_FILENO, args[0], strlen(args[0]));
write(STDERR_FILENO, ": not found\n", 12);
free(full_path);
exit(127);
}
}
else if (pid > 0)
{
/* Parent process - wait for child to complete */
wait(&status);
free(full_path);
}
else
{
/* Fork failed */
perror("fork");
free(full_path);
}

return (1);
}
