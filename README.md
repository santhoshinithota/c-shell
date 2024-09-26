# Project: C Shell

## Introduction
This project implements a custom shell that supports various commands like `hop`, `reveal`, `log`, `seek`, and system commands, among other functionalities. The shell follows a custom prompt format and allows background and foreground process execution.

## Files and Structure

- **Main Files**
  - `main.c`: The entry point of the shell that handles user input and command execution.
  - `makefile`: Compiles the project.

- **Command Implementations**
  - **hop command**:
    - `handleHome.c`, `handleHome.h`: Implements the `hop` command, which changes directories.
    - Supports the flags `.` (current directory), `..` (parent directory), `~` (home directory), and `-` (previous directory).
  - **reveal command**:
    - `ls.c`, `ls.h`: Implements the `reveal` command, similar to `ls`, with `-a` and `-l` flags for listing all files and showing detailed information, respectively. Color coding is also applied to distinguish file types.
  - **log command**:
    - `history.c`, `history.h`: Implements the `log` command, storing up to 15 commands across sessions, with support for `log purge` and `log execute <index>`.
  - **proclore command**:
    - `proclore.c`, `proclore.h`: Implements the `proclore` command to display process information such as PID, status, group, virtual memory, and executable path.
  - **seek command**:
    - (File not explicitly mentioned, assumed to be part of `main.c` or an additional module for handling the seek functionality as described).

- **Supporting Files**
  - `prompt.c`, `prompt.h`: Manages the shell prompt, showing the username, system name, and current directory, with paths relative to the home directory when applicable.
  - `death.c`, `death.h`: Handle termination of background processes and displaying exit messages.
  - `headers.h`: Contains common includes and definitions used across the project.

- **Sample and Test Files**
  - `sample`, `white.txt`: Test or configuration files used during development.
  - `shell`: The compiled shell executable.

## Features

### 1. Shell Prompt
- Displays in the format `<Username@SystemName:~>`, with the current directory path relative to the shell's home directory. Changes dynamically when the working directory changes.

### 2. Command Input
- Supports command sequences separated by `;` or `&`, handling random spaces and tabs. Background processes are indicated by their process IDs, and errors are printed for invalid commands.

### 3. hop Command
- Changes the current directory with support for flags like `.` (current directory), `..` (parent directory), `~` (home directory), and `-` (previous directory).
- Executes multiple `hop` commands sequentially when multiple arguments are provided.

### 4. reveal Command
- Lists files and directories with `-a` (show hidden files) and `-l` (show detailed info) flags.
- Supports both relative and absolute paths, with color coding for file types (green for executables, white for files, blue for directories).

### 5. log Command
- Stores the history of the last 15 commands, excluding identical consecutive commands and the `log` command itself. Supports `log purge` and `log execute <index>` functionalities.

### 6. System Commands
- Executes system commands with support for foreground and background execution. Background processes automatically print messages when they complete.

### 7. proclore Command
- Displays process information including PID, status, process group, virtual memory, and executable path.

### 8. seek Command
- Searches for files or directories in the specified target directory. Supports flags like `-d` (only directories), `-f` (only files), and `-e` (print file content or change to directory if only one match is found). Provides error messages for invalid flag combinations.

## Assumptions
- File and directory names do not contain whitespace characters.

## Compilation and Execution
1. Use `make` to compile the project.
2. Run the shell using the `./shell` command.
