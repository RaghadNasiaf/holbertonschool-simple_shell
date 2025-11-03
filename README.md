# Simple Shell - 0.3

**Author:** Raghad Naseif (<naseefraghad@gmail.com>)

This project implements a minimal UNIX command-line interpreter.

### Features
- Displays a prompt and waits for the user to type a command.
- Handles command lines with arguments.
- Handles the `PATH` to find programs.
- Does not call `fork()` if the command does not exist.
- Works in both interactive and non-interactive modes.
- Exits cleanly when the user types `exit`.

### Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

