# Simple Shell

A UNIX command line interpreter written in C as part of the Holberton School curriculum.

## Description
This project is a collaboration between Holberton School students to create a simple shell that mimics basic shell functionality. The shell can execute commands, handle built-in functions, manage processes, and handle both interactive and non-interactive modes.

## Requirements
- All files compiled on Ubuntu 20.04 LTS using `gcc` with flags: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Code follows Betty style guidelines
- No memory leaks
- No more than 5 functions per file
- All header files include guarded
- Output must match `/bin/sh` exactly
- Use system calls only when necessary

## Features
- Display prompt and wait for user input
- Execute commands with arguments
- Handle built-in commands (`exit`, `env`)
- Search for executables in PATH
- Handle end of file (Ctrl+D)
- Error handling that matches `/bin/sh`
- Both interactive and non-interactive modes
- Proper exit status handling
- Memory leak free

## Allowed Functions
- All functions from `string.h`
- `access`, `chdir`, `close`, `closedir`
- `execve`, `exit`, `_exit`
- `fflush`, `fork`, `free`
- `getcwd`, `getline`, `getpid`
- `isatty`, `kill`, `malloc`
- `open`, `opendir`, `perror`
- `printf`, `fprintf`, `vfprintf`, `sprintf`, `putchar`
- `read`, `readdir`, `signal`
- `stat`, `lstat`, `fstat`
- `strtok`, `wait`, `waitpid`, `wait3`, `wait4`
- `write`

## Project Tasks Completed

### Task 0: README, man, AUTHORS
- **README.md**: Project documentation
- **man_1_simple_shell**: Manual page for the shell
- **AUTHORS**: List of project contributors

### Task 1: Betty would be proud
- Code follows Betty style guidelines
- Proper function descriptions and formatting
- No more than 5 functions per file
- Header include guards

### Task 2: Simple shell 0.1
- Basic shell functionality
- Display prompt and read commands
- Execute single-word commands with full paths
- Handle EOF (Ctrl+D)
- Error handling for non-existent commands

### Task 3: Simple shell 0.2
- Handle command lines with arguments
- Execute commands like `ls -l /tmp`
- Improved tokenization for multiple arguments

### Task 4: Simple shell 0.3
- Handle PATH environment variable
- Search for commands in PATH directories
- Don't call fork if command doesn't exist
- Handle empty PATH correctly

### Task 5: Simple shell 0.4
- Implement `exit` built-in command
- Exit without arguments
- Proper exit status handling

### Task 6: Simple shell 1.0
- Implement `env` built-in command
- Print current environment variables
- Built-in returns status 0

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
Usage
Interactive Mode
bash
./hsh
Non-interactive Mode
bash
echo "command" | ./hsh
Examples
Interactive Mode Example
bash
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($) exit
$
Non-interactive Mode Example
bash
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c
$ cat commands.txt | ./hsh
hsh main.c shell.c
Error Handling Examples
bash
$ ./hsh
($) qwerty
./hsh: 1: qwerty: not found
($) ./././hsh
./././hsh: 1: ./././hsh: not found
($) exit
Built-in Commands
bash
$ ./hsh
($) env
USER=username
HOME=/home/username
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
...
($) exit
File Structure
text
holbertonschool-simple_shell/
├── AUTHORS              # List of contributors
├── README.md           # Project documentation
├── man_1_simple_shell  # Manual page
├── shell.h             # Header file with prototypes
├── main.c              # Main program and shell loop
└── shell.c             # Core shell functionality
Functions
main.c
main(): Entry point and main shell loop

Signal handling for Ctrl+C

shell.c
display_prompt(): Displays shell prompt

read_input(): Reads input from stdin

parse_input(): Splits input into tokens

execute_command(): Executes commands with PATH resolution

free_args(): Frees memory allocated for arguments

_getenv(): Custom getenv implementation

find_command(): Searches for commands in PATH

command_exists(): Checks if command exists and is executable

print_error(): Prints formatted error messages

print_environment(): Prints environment variables

Environment
The shell inherits environment variables from the parent process and can access them through the global environ variable.

Error Handling
Commands not found: ./hsh: 1: command: not found (exit status 127)

Commands that fail: Returns the command's exit status

Empty input: Continues without error

EOF (Ctrl+D): Exits cleanly

Signal Handling
Ctrl+C: Ignored (shows new prompt)

Ctrl+D: Exits shell

Memory Management
All dynamically allocated memory is properly freed

No memory leaks (verified with valgrind)

Safe string operations

Limitations
No pipes (|) support

No redirections (>, <, >>) support

No logical operators (&&, ||) support

No variable expansion

No command history

No tab completion

Testing
The shell has been tested to match /bin/sh output exactly in both interactive and non-interactive modes.

Authors
See AUTHORS file for complete list of contributors.

Manual
View the manual with:

bash
man ./man_1_simple_shell
License
This project is part of the Holberton School curriculum.
