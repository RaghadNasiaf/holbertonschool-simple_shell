#include "shell.h"

/**
* main - Simple shell entry point
*
* Return: Exit status of the last command
*/
int main(void)
{
char *input = NULL;
char **args = NULL;
int status = 0;
int last_status = 0;

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

/* Check if it's exit command before executing */
if (strcmp(args[0], "exit") == 0)
{
free_args(args);
break;
}

/* Check if it's env command */
if (strcmp(args[0], "env") == 0)
{
print_environment();
free_args(args);
last_status = 0;  /* env command returns 0 */
continue;
}

status = execute_command(args);
free_args(args);
args = NULL;

last_status = status;  /* Store the last command status */

} while (1);

return (last_status);
}
