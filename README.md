# Minishell  

Minishell is a simple shell implementation written in C, designed to replicate the behavior of a Unix shell. The project focuses on creating a functional shell environment, handling user input, managing processes, and implementing a subset of shell features. It’s a hands-on introduction to system programming and the inner workings of command-line interpreters.

## How It Works  

1. **User Input and Parsing**  
   - The shell reads user input, processes it, and parses commands and arguments.  
   - Supports features like:  
     - Quoting (`'` and `"`) for strings with spaces or special characters.  
     - Environment variable expansion (`$VARIABLE`).  
     - Redirection (`>`, `<`, `>>`) for input/output management.  
     - Pipes (`|`) for chaining commands.  

2. **Built-in Commands**  
   - Implements core shell commands, including:  
     - `cd`: Change directory.  
     - `echo`: Print text to standard output.  
     - `pwd`: Display the current working directory.  
     - `export` and `unset`: Manage environment variables.  
     - `env`: Display environment variables.  
     - `exit`: Exit the shell.  

3. **Process Management**  
   - Executes non-built-in commands by forking processes and using `execve` to run the specified program.  
   - Manages processes and file descriptors to support pipelines and redirections.  

4. **Error Handling**  
   - Provides meaningful error messages for invalid commands, incorrect syntax, or inaccessible files.  
   - Ensures proper cleanup of resources to avoid memory leaks and orphaned processes.  

5. **Signal Handling**  
   - Handles signals like `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` to mimic shell behavior:  
     - `Ctrl+C`: Interrupts the current process.  
     - `Ctrl+D`: Exits the shell when no input is provided.  
     - `Ctrl+\`: Sends a quit signal.  

## What I Learned  
- How shells interpret and execute commands in a Unix-like environment.  
- Parsing and tokenizing user input to handle complex syntax like pipes and redirections.  
- Managing child processes and inter-process communication using `fork`, `execve`, and pipes.  
- Using file descriptors to implement input/output redirection and pipelines.  
- Signal handling to ensure the shell behaves consistently under various inputs.  
- Debugging and ensuring robust error handling in a low-level programming context.  
