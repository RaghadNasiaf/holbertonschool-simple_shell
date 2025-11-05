#include "shell.h"

/**
* main - Simple shell entry point
*
* Return: Always 0
*/
int main(void)
{
char *input;
char **args;
int status = 1;

do {
display_prompt();
input = read_input();
if (!input)
break;

args = parse_input(input);
if (!args[0])
{
free(input);
free(args);
continue;
}

status = execute_command(args);

free(input);
free(args);
} while (status);

return (0);
}
