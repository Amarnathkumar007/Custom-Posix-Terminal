#include <iostream>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "redirection.hpp"
#include "util.hpp"

#define nl '\n'


Redirect::Redirect() {
    //Constructor Implementation

};

Redirect::~Redirect() {
    //Destructor Implementation
}



void Redirect::in_redirection (char *fileName) {
    int input_fd = open(fileName, O_RDONLY);
    dup2(input_fd, 0);
    close(input_fd);
}


void Redirect::out_redirection (char *fileName) {
    int output_fd = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(output_fd, 1);
    close(output_fd);
}


void Redirect::run_redirection(char *arguments[]) {
    pid_t process_id;
    if (strcmp(arguments[0], "exit") == 0) {
        return;
    }
    else {
        process_id = fork();
        if (process_id < 0) { 
            cout << "Fork() failed...Unable to create process" << nl;
        }
        else if (process_id == 0) {
            //Child
            execvp(arguments[0], arguments);
        }
        else {
            //Parent
        }
            Redirect::in_redirection ("/dev/tty");
            Redirect::out_redirection ("/dev/tty");
    }
}



string Redirect::string_tokenize (string & input_command) {
    int i = 0;
    string tokenized_string = "";

    for (int i = 0; i < input_command.size(); i++) {
        if (input_command[i] != '>' && input_command[i] != '<') {
            tokenized_string += input_command[i];
        }
        else {
            tokenized_string += ' ';
            tokenized_string += input_command[i];
            tokenized_string += ' ';
        }
    }
    return tokenized_string;

}



bool Redirect::get_console_output (string& input_command) {
    const int MAX_ARGUMENTS = 10;

    char* arguments[MAX_ARGUMENTS];
    string tokenized_str = Redirect::string_tokenize(input_command);
    char* c_tokenized_str = Util::convert_to_cstr(tokenized_str);
    
    int i = 0;
    char* argument = strtok(c_tokenized_str, " ");
    while (argument != NULL) {
        if (strcmp(argument, ">") == 0) {
            Redirect::out_redirection(strtok(NULL, " "));
        }
        else if (strcmp(argument, "<") == 0) {
            Redirect::in_redirection(strtok(NULL, " "));
        }
        else {
            arguments[i] = argument;
            ++i;
        }
        argument = strtok(NULL, " ");
    }
    arguments[i] = NULL;

    Redirect::run_redirection(arguments);

    return true;

} 

