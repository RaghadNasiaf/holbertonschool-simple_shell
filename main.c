#include "shell.h"

/**
* handle_signal - Handles Ctrl+C signal
* @sig: Signal number
*/
void handle_signal(int sig)
{
(void)sig;
write(STDOUT_FILENO, "\n", 1);
display_prompt();
fflush(stdout);
}

/**
* main - Simple shell entry point (Task 3)
*
* Return: Always 0
*/
int main(void)
{
char *input;
char **args;
int status = 1;

/* Set up signal handling for Ctrl+C */
signal(SIGINT, handle_signal);

do {
display_prompt();
input = read_input();

/* Handle EOF (Ctrl+D) */
if (!input)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
break;
}

args = parse_input(input);
if (!args || !args[0])
{
free(input);
if (args)
free(args);
continue;
}

status = execute_command(args);

free(input);
free(args);
} while (status);

return (0);
}
