#include "shell.h"

/**
* main - Simple shell entry point
*
* Return: Always 0
*/
int main(void)
{
char *input = NULL;
char **args = NULL;
int status = 1;

signal(SIGINT, SIG_IGN);

do {
display_prompt();
input = read_input();

if (!input)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
break;
}

args = parse_input(input);
free(input);
input = NULL;

if (!args || !args[0])
{
free_args(args);
continue;
}

status = execute_command(args);
free_args(args);
args = NULL;

} while (status);

return (0);
}
