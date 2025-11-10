# Custome Posix Terminal

##### Implemented a shell that supports a semi-colon separated list of commands. We need to use 'strtok' to tokenize the command. Also, support '&' operator which lets a program run in the background after printing the process id of the newly created process. 


## How to Run?
1. A makefile along with all the source files have been included in the assignment submission
1. Run `make` command to compile and link the shell files
2. Run `./main` to execute the main shell program

## Features of the shell

1. Many commands are inbuilt which are explained in the following section. All the other commands which are to be executed in foreground and/or background are implemented using `execvp`. So, all the commands available in Bash can be executed as well in this shell.

2. The prompt for taking the next command displays the `username`, `hostname` and the `present working directory`. The directory in which the shell is run is taken to be the home directory `~` of the shell. The present working directory is displayed either in terms of `/` (if present working directory is the root of this shell) or as absolute path.

3. This shell implements the usual commands of the linux shell like `cd`, `echo` and `pwd`. Variations of these commands are also supported by this shell. 

4. `ls` command has also been implemented along with its two flags namely `-a` and `-l`, along with '.', '..', '~', and directory paths as arguments.

5. This shell also supports creating foregroud and background processes with the use of `&` symbol.

6. `pinfo` is included as a command to display some specific information about the current self process or any other process when supplied with that process' PID.

7. `search` command searches for the presence of specific files recursively starting from the current working directory.

8. This shell also supports IO redirection using symbols `<`, `>` and `>>`.

9. This shell also supports pipelining as in the original bash shell. Multiple commands can be chained together using the pipe `|` symbol to pipe the output of one command to the input of another.

10. Support for simple signals like <kbd>CTRL</kbd> + <kbd>Z</kbd> (SIGTSTP), <kbd>CTRL</kbd> + <kbd>C</kbd>(SIGINT) and <kbd>CTRL</kbd> + <kbd>D</kbd>(EOT) have been included with their appropriate behaviour

11. Autocomplete functionality for commands as well as files have been included to help the user with autocomplete functionality. Appropriate error messages are displayed when autocomplete does not find matching commands, files or directories.

12. `history <num>` command implements the history feature which prints 'num' number of previous commands entered by the user. Maximum storage capacity of history is 20 and maximum commands output is 10 (by default)

## Inbuilt Commands

1. `echo`
    * Implemented in `echo.cpp`
    * Takes a string argument and prints it after removing the extra spaces.

2. `pwd`
    * Implemented in `pwd.cpp`
    * Prints the path of current working directory.
    * Uses the `getcwd()` system call.

3. `cd [location]`
    * Implemented in `cd.cpp`
    * Changes the current working directory to the mentioned directory. If no parameter is given, it changes the directory to the root directory of the shell.
    * If `~` is present in the given `location`, it is replaced with the home directory of the shell.
    * If `location` is `-`, it is interpreted as the previous working directory of the shell.
    * Implemented using `chdir()` system call.

4. `ls [-l -a -al -la] [Directory]`
    * Implemented in `ls.cpp`
    * Lists all the files and directories in the mentioned directory/directories. If no parameters are passed, lists the contents of current directory.
    * `-l` flag lists the long format of `ls`, providing additional details such as permissions, owner, time of creation etc.
    * `-a` flag includes the hidden files/diectories in the listing.
    * The flags and directories can be provided in any order.
    * Uses the `readdir()` system call.

5. `pinfo [process_id]`
    * Implemented in `pinfo.cpp`
    * Gives the information about `process_id` process. If `process_id` not mentioned, gives information about the current process.
    * The information includes Process ID, Process Name, State of the process and the exceutable path of the process.
    * Uses the files `/proc/process_id/status` and `/proc/process_id/exec` to fetch the required information. 

6. `history [num]`
    * Implemented in `history.cpp`
    * Gives the `num` number of previous commands run. If `num` is not mentioned, 10 is taken as the default value for `num`.
    * Continous repetitions, invalid commands and blank lines are avoided in the history.

12. `fg`
    * Implemented in `main.cpp`
    * Makes a foreground process and blocks the shell until the process exits or is forced to exit

13. `bg_process &`
    * Implemented in `background.cpp`
    * Creates a background process and resumes the terminal for command processing
    * 
15. `exit`
    * Implemented in `main.cpp`
    * Logs out of the terminal.
    * Use this command to ensure proper closing (killing all persisting background processes).

## Executional Details     

1. Any command ending with `&` is treated as a background process the shell does not wait for its execution. If such a process requests terminal control, it will automatically suspended in the background. The shell keeps track of all background processes and alerts the user on their completion.

2. <kbd>|</kbd> is used for piping of commands, i.e, Output of one command serves as input for the next.
Example:
    ```
    username@hostname $ cat todo.txt | head -7 | tail -5
    ```

3. <kbd><</kbd> is used for input redirection. <kbd>></kbd> (for overwriting) and <kbd>>></kbd> (for apending) are used for output redirection.<br>
Example:
    ```
    # Input Redirection
    username@hostname $ cat < todo.txt 

    # Output Redirection
    username@hostname $ ls -l > list_dir.txt

    # Input and Output Redirection
    username@hostname $ cat < lines.txt > lines_copy.txt
    ```

5. <kbd>Ctrl</kbd> + <kbd>C</kbd> sends the `SIGINT` signal and terminates any foreground process.

6. <kbd>Ctrl</kbd> + <kbd>Z</kbd> sends the `SIGSTP` signal and suspends any running foreground process.

7. <kbd>Ctrl</kbd> + <kbd>D</kbd> is an `EOF` character and terminates the shell.


### To Do
1. UP arrow key for command-recall
2. Handle edge case of echo command
3. pinfo - add + for foreground
4. fix encoding for GUI apps when started with execv()
5. Implement redirection with pipes







