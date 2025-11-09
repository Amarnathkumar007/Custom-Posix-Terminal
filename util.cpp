#include <iostream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.hpp"
#include "ls.hpp"

#define nl '\n'

using namespace std;


Util::Util() {
    //Constructor Implementation

};

Util::~Util() {
    //Destructor Implementation
}


pid_t Util::foreground_process_id;

//Function Implementation

char* Util::convert_to_cstr(string& input_str) {
    int size = input_str.size();
    char* output_str = (char*)malloc((size + 1)* sizeof(char));

    for (int i = 0; i < size; i++) {
        output_str[i] = input_str[i];
    }
    output_str[size] = '\0';

    return output_str;

}


vector<string> Util::tokenize_string (string& input_str, const char* sep) {
    char* c_input_string = convert_to_cstr(input_str);
    char* token;
    vector<string> tokenized_str;

    token = strtok(c_input_string, sep);
    while (token != NULL) {
        // cout << token << nl;
        tokenized_str.push_back(string(token));
        token =  strtok(NULL, sep);
    }

    return tokenized_str;

}



string Util::get_parent_directory_path (string& current_pwd_path) {
    const char forward_slash_sep[] = {"/"};
    vector<string> tokenized_output = tokenize_string(current_pwd_path, forward_slash_sep);
    string output_str = "/";

    for (int i = 0; i < (int)tokenized_output.size() - 1; i++) {
        output_str += tokenized_output[i] + "/";
    }

    return output_str;

}



bool Util::validate_directory_path(string& dir_path) {
    struct stat sb;
    char* c_dir_path = convert_to_cstr(dir_path);

    if (stat(c_dir_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    } 
    else {
        return false;
    }

}


bool Util::validate_file_path(string& file_path) {
    struct stat sb;
    char* c_dir_path = convert_to_cstr(file_path);

    if (stat(c_dir_path, &sb) == 0 && S_ISREG(sb.st_mode)) {
        return true;
    } 
    else {
        return false;
    }

}


string Util::append_space (string& input_str, int n) {
    return input_str += string(n, ' ') ;
}


int Util::int_len (int num) {
    int len = 0;
    while (num) {
        num /= 10;
        ++len;
    }
    return len;
}


string Util::get_file_name_from_path(string& final_file_path) {
    const char forward_slash_sep[] = {"/"};
    vector<string> tokenized_output = Util::tokenize_string(final_file_path, forward_slash_sep);
    return tokenized_output[tokenized_output.size() - 1];
}


bool Util::starts_with(char* c_input_str, const string& prefix) {
    for (int i = 0; i < (int)min(strlen(c_input_str), (size_t)prefix.size()); i++) {
        if (c_input_str[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}


bool Util::starts_with_prefix(const string& str1, string& str2) {
    for (int i = 0; i < min(str1.size(), str2.size()); i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }

    return true;
}


int Util::extract_num_from_str(string& input_str) {
    int i = 0;
    for (i = 0; i < (int)input_str.size(); i++) {
        if (isdigit(input_str[i])) {
            break;
        }
    }

    int j = i;
    for(j = i; j < (int)input_str.size(); j++) {
        if (!isdigit(input_str[i])) {
            break;
        }
    }

    return stoi(input_str.substr(i, j - i));

}

bool Util::sortingPolicy (pair<string, string>& a, pair<string, string>& b) {
    return a.first < b.first;
}

bool Util::is_command_valid (string& command, unordered_map<string, bool>& valid_commands) {
    if (valid_commands.find(command) != valid_commands.end()) {
        return true;
    }
    return false;
}


bool Util::is_num (string& input) {
    for (int i = 0; i < (int)input.size(); i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

string Util::trim_trailing_spaces (string& input_str) {
    int i = 0;
    for (i = input_str.size() - 1; i >= 0; i--) {
        if (input_str[i] != ' ') {
            break;
        }
    }
    return input_str.substr(0, i + 1);
}

void Util::trim_new_line (string& input_str) {
    input_str.erase(std::remove(input_str.begin(), input_str.end(), '\n'), input_str.end());
}


pair<string, string> Util::get_input_from_raw_terminal() {
    // cout << "Reading Raw" << endl;
    pair<string, string> output;
    struct termios initial_terminal;
    struct termios raw_terminal;
    // Util::set_raw_mode(initial_terminal); // Switch terminal to raw mode
    tcgetattr(STDIN_FILENO, &initial_terminal);
    raw_terminal = initial_terminal;
    raw_terminal.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw_terminal);

    string input;
    char input_char;
    while (true) {
        input_char = getchar();
        // input += input_char;
        putchar(input_char);

        if (input_char == '\t') {
            //Handle TAB key press
            output.first = "tab";
            output.second = input; 
            break;
        } 
        else if (input_char == '\n') {
            output.first = "newl";
            // getline(cin, output.second);
            output.second = input;
            break;
        }
        else if (input_char == 127) {
            if (input.size() > 0) {
                input.erase(input.size() - 1);
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        }
        else if (input_char == '\x04') {
            // cout << "CTRL+D received" << endl;
            output.first = "exit";
            break;
        }
        else {
            input += input_char;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &initial_terminal);

    return output;

}

vector<string> Util::get_autocomplete (string& input_string, unordered_map<string, bool>& valid_commands, string& current_pwd_path) {
    if (input_string[input_string.size() - 1] == '\t') {
        // input_string.erase(input_string.find(input_string.begin(), input_string.end(), '\t'), input_string.end());
        input_string.erase(std::remove(input_string.begin(), input_string.end(), '\t'), input_string.end());
    }
    vector<string> tokenized_args = Util::tokenize_string(input_string, " ");
    vector<string> output;

    if (tokenized_args.size() == 1) {

        //Complete command
        for (auto& it : valid_commands)  {
            if (Util::starts_with_prefix(it.first, tokenized_args[0])) {
                // cout << "Matched" << endl;
                output.push_back(it.first);
                // cout << it.first << " ";
            }
        }
        if (output.empty()) {
            output.push_back("error");
            output.push_back("No matching commands found");
        }
        return output;
    }
    else if (tokenized_args.size() == 2) {

        //Complete matching file_name
        vector<string> dir_content_list = Ls::list_directory(current_pwd_path, false, false);

        for (int i = 0; i < dir_content_list.size(); i++) {
            if (Util::starts_with_prefix(dir_content_list[i], tokenized_args[1])) {
                output.push_back(dir_content_list[i]);
            }
        }
        if (output.empty()) {
            output.push_back("error");
            output.push_back("No matching files/directories found");
        }
        return output;

    }
    else {
        output.push_back("error");
        output.push_back("Invalid arguments to autocomplete function");
    }

    return output;

}


bool Util::contains_pipe (string& input_command) {
    return (input_command.find('|') == string::npos) ? 0 : 1;
}


string Util::trim_leading_and_trailing_space (string& input_str) {
    int i, j;
    for (i = 0; i < input_str.size(); i++) {
        if (input_str[i] != ' ') {
            break;
        }
    }
    for (j = input_str.size() - 1; j >= 0; j--) {
        if (input_str[j] != ' ') {
            break;
        }
    }
    return input_str.substr(i, j - i + 1);
} 

char** Util::convert_to_c_array (vector<string>& input_vector) {
    char** output_array = (char **)malloc(input_vector.size() * sizeof(char*));
    for (int i = 0; i < input_vector.size(); i++) {
        output_array[i] = Util::convert_to_cstr(input_vector[i]);
    }
    return output_array;
}


bool Util::contains_background(string& input_string) {
    return (input_string.find('&') == string::npos) ? 0 : 1;
}

vector<string> Util::extract_command_from_bg_command (string& input_string) {
    vector<string> tokenized_string_temp = Util::tokenize_string(input_string, " ");
    // tokenized_string.erase(tokenized_string.rbegin());
    vector<string> tokenized_string;
    for (int i = 0; i < tokenized_string_temp.size() - 1; i++) {
        tokenized_string.push_back(tokenized_string_temp[i]);
    }

    return tokenized_string;
}


bool Util::create_foreground_process (vector<string>& tokenized_string_temp) {
    int status = 0;
    char** c_args = Util::convert_to_c_array(tokenized_string_temp);
    
    pid_t child_pid;
    if ((child_pid = fork()) < 0) {
        cout << "Error in creating foreground process" << nl;
        return false;
    } 
    else if (child_pid == 0) {
        //Child
        execvp(c_args[0], c_args);
        cerr << c_args[0] << " : command not found";
        exit(1);
    }
    else {
        //Parent
        foreground_process_id = child_pid;
        waitpid(child_pid, &status, 0);
        foreground_process_id = 0;
        cout << "Child exited with status " << status << nl; 
    }

    return true;

}


bool Util::contains_redirection (string& input_string) {
    if (input_string.find('>') != string::npos || input_string.find('<') != string::npos) {
        return true;
    }
    return false;
}