# My simple Shell

## Description
This repository contains a simple UNIX command line interpreter written in C.  
It is a Holberton School project designed to teach low‑level programming, system calls, process creation, and memory management.

The shell provides an environment to execute commands, handle built‑ins, and manage PATH resolution.

## Features
Displays a prompt and waits for user's input.  
Executes commands with arguments.  
Handles absolute and relative paths.  
Searches for executables in the PATH environment variable.  
Built‑ins:  
  * exit → exits the shell.  
  * env → prints the current environment.  
Proper error handling:  
  * Prints `./hsh: 1: command: not found` when a command does not exist.  
  * Returns exit status `127` for command not found.  
Works in both interactive and non‑interactive modes.

## Compilation
You can compile this with:

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`

### Thank you.
