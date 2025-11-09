#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "pipe.hpp"
#include "util.hpp"

#define MAX_COMMANDS 10
#define MAX_ARGS 10
#define BUFFER_SIZE 1024


#define nl '\n'

using namespace std;


Pipe::Pipe() {
    //Constructor Implementation

};

Pipe::~Pipe() {
    //Destructor Implementation
};



void Pipe::parse_command(char *command, char **args) {
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void execute_command(char **args, int input_fd, int output_fd) {
    pid_t pid;

    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (input_fd != -1) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != -1) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Parent process
    if (input_fd != -1) {
        close(input_fd);
    }
    if (output_fd != -1) {
        close(output_fd);
    }
    wait(NULL);
}


bool Pipe::run_pipeline(char *commands[], int num_commands) {
    int pipefds[2];
    int in_fd = 0;


    for (int i = 0; i < num_commands; ++i) {
        pipe(pipefds);

        if (fork() == 0) {
            
            dup2(in_fd, 0); 
            if (i < num_commands - 1) {
                dup2(pipefds[1], 1);
            }

            close(pipefds[0]); 

            char *args[MAX_ARGS];
            Pipe::parse_command(commands[i], args);

            execvp(args[0], args); 
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } 
        else {
            // Parent process
            wait(NULL);
            close(pipefds[1]);
            in_fd = pipefds[0];
        }
    }

}



bool Pipe::get_console_output (string& input_command) {
    vector<string> tokenized_on_pipe = Util::tokenize_string(input_command, "|");
    char * commands[10];
    int i = 0;
    for (auto& str : tokenized_on_pipe) {
        commands[i++] = Util::convert_to_cstr(str);
    }

    return Pipe::run_pipeline(commands, i);


}
