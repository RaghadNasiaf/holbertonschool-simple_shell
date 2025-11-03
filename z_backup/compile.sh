#!/usr/bin/env bash
set -e
gcc -Wall -Werror -Wextra -pedantic simple_shell.c -o hsh
echo "Build OK -> ./hsh"
