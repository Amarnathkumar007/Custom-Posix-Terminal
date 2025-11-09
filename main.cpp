#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include "system_interface.hpp"
#include "util.hpp"
#include "echo.hpp"
#include "cd.hpp"
#include "pwd.hpp"
#include "ls.hpp"
#include "pinfo.hpp"
#include "search.hpp"
#include "sig_handler.hpp"
#include "history.hpp"
#include "pipe.hpp"
#include "background.hpp"
#include "redirection.hpp"

#define nl '\n'

using namespace std;


class FrazUtil {

    public:

        static string to_lowercase (string& input_command) {
            const int MAX_INPUT_SIZE = 50;
            string lower_input_command; 
            lower_input_command.reserve(50);

            for (int i = 0; i < min(MAX_INPUT_SIZE, (int)input_command.size()); i++) {
                lower_input_command.push_back(tolower(input_command[i]));
            }
            // cout << lower_input_command << nl;
            return lower_input_command;
        }

        static bool is_exit (string& input_command) {
            if (to_lowercase(input_command) == "exit") {
                return true;
            }
            else return false;
        }

        static char* convert_to_cstr(string& input_str) {
            int size = input_str.size();
            char* output_str = (char*)malloc((size + 1)* sizeof(char));

            for (int i = 0; i < size; i++) {
                output_str[i] = input_str[i];
            }
            output_str[size] = '\0';

            // cout << output_str << nl;
            return output_str;

        }
        

        static vector<string> tokenize_string (string& input_str, const char* sep) {
            char* c_input_string = FrazUtil::convert_to_cstr(input_str);
            char* token;
            vector<string> tokenized_str;

            token = strtok(c_input_string, sep);
            while (token != NULL) {
                // cout << token << nl;
                tokenized_str.push_back(string(token));
                token =  strtok(NULL, sep);
            }

            for (auto& p : tokenized_str) {
                cout << p << nl;
            }

            return tokenized_str;

        }

        static string trim_start(string& input_string) {
            int i;
            for (i = 0; i < (int)input_string.size(); i++) {
                if (input_string[i] != ' ') {
                    break;
                }
            }
            input_string = input_string.substr(i, (int)input_string.size() - i);

            return input_string;

        }
        

        static string trim_end(string& input_string) {
            int i;
            for (i = (int)input_string.size() - 1; i >= 0; i--) {
                if (input_string[i] != ' ') {
                    break;

                }
            }
            input_string = input_string.substr(0, i + 1);

            return input_string;
            
        }


        static pair<string, string> extract_command_args(string input_string) {
            string command = "";
            string args = "";

            int i;
            for (i = 0; i < (int)input_string.size(); i++) {
                if (input_string[i] == ' ') {
                    break;
                }
            }
            command = input_string.substr(0, i);
            // if (i == input_string.size()) {
            //     command = 

            // }
            while(i < (int)input_string.size() && input_string[i] == ' ') {
                ++i;
            }
            args = input_string.substr(i, input_string.size() - i);

            return {command, args};

        }


        static bool validate_pwd_args (string& args) {
            return (args.size() == 0) ? true : false;
        }


        static bool validate_directory_path(string& dir_path) {
            struct stat sb;
            char* c_dir_path = convert_to_cstr(dir_path);

            if (stat(c_dir_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                return true;
            } 
            else {
                return false;
            }

        }


        static string get_parent_directory_path (string& current_pwd_path) {
            const char forward_slash_sep[] = {"/"};
            vector<string> tokenized_output = FrazUtil::tokenize_string(current_pwd_path, forward_slash_sep);
            string output_str = "/";

            for (int i = 0; i < (int)tokenized_output.size() - 1; i++) {
                output_str += tokenized_output[i] + "/";
            }

            return output_str;

        }

};



int main (void) {

    const int UNAME_BUFF_SIZE = 100;
    const int SYSNAME_BUFF_SIZE = 100;
    const int PWD_BUFF_SIZE = 150;

    unordered_map<string, bool> valid_commands = {
        {"echo", true},
        {"pwd", true},
        {"cd", true},
        {"ls", true},
        {"pinfo", true},
        {"search", true},
        {"history", true}
    };

    string user_name = SystemInterface::get_login_user_name(UNAME_BUFF_SIZE);
    string sys_name = SystemInterface::get_system_name(SYSNAME_BUFF_SIZE);
    string home_pwd_path = SystemInterface::get_pwd(PWD_BUFF_SIZE);
    string current_pwd_path = home_pwd_path;
    string prev_pwd_path = home_pwd_path;
    string prompt_path_display = "";

    const std::string cyan("\033[96m");
    const std::string red("\033[91m");
    const std::string reset("\033[0m");

    if (!History::read_from_file()) {
        cout << "Unable to fetch history" << nl;
    }
    else {
        // History loaded successfully
        // cout << "History loaded" << nl;
    }

    string def_shell_prompt = user_name + "@" + sys_name + ":~";

    const char space_sep[] = {" "};
    // char forward_slash_sep[1] = {'/'};


    while (1) {
        // mainloop:
        signal(SIGINT, SignalHandler::sigint_handler);
        signal(SIGTSTP, SignalHandler::sigtstp_handler);

        cout << cyan << def_shell_prompt << red << prompt_path_display << reset << ">";
        cout.flush();

        string input_command_full;

        pair<string, string> raw_input_command = Util::get_input_from_raw_terminal();

        if (raw_input_command.first == "tab") {
            vector<string> output = Util::get_autocomplete(raw_input_command.second, valid_commands, current_pwd_path);

            cout << nl;
            if (output.size() == 0) {
                cout << "No matching commands";
            }
            else if (output[0] == "error") {
                cout << output[1] << nl;
                continue;
            }
            for (auto& p : output) {
                cout << p << nl;
            }
            continue;
        }

        if (raw_input_command.first == "newl") {
            input_command_full = raw_input_command.second;
            Util::trim_new_line(input_command_full);
        }
        // getline(cin, input_command);

        // if (cin.eof()) {
        //     cout << "Logging out..." << nl;
        //     // exit(EXIT_SUCCESS);
        //     goto end;
        // }

        if (raw_input_command.first == "exit") {
            cout << "Logging out..." << nl;
            // exit(EXIT_SUCCESS);
            goto end;
        }



        vector<string> tokenized_on_semicolon = Util::tokenize_string(input_command_full, ";");

        for (string input_command : tokenized_on_semicolon) {

            if (input_command.size() == 0) {
                continue;
            }
            // else if (raw_input_command)


            // cout << input_command << nl;

            FrazUtil::trim_start(input_command);
            FrazUtil::trim_end(input_command);
            

            // **** Handle EXIT command
            if (FrazUtil::is_exit(input_command)) {
                string exit_string = "exit";
                History::push_command(exit_string);
                goto end;
            }


            // **** Handle PIPE ****
            if (Util::contains_pipe(input_command)) {
                bool yes = Pipe::get_console_output(input_command);

                continue;
            }


            // **** Handle Redirection
            if (Util::contains_redirection(input_command)) {
                bool yes = Redirect::get_console_output(input_command);
                continue;
            }


            // **** Handle Background ****
            if (Util::contains_background(input_command)) {
                bool yes = Background::get_console_output(input_command);
                continue;
            }


            //Process input command

            pair<string, string> cmd_args = FrazUtil::extract_command_args(input_command);

            string command = cmd_args.first;
            string args = cmd_args.second;

            // cout << "command: " << command << nl << "args: " << args << nl;

            if (Util::is_command_valid(command, valid_commands)) {
                History::push_command(command);
            }

            // **** Echo command Handler ****
            if (command == "echo") {
                FrazUtil::trim_start(args);
                cout << Echo::echo_handler(args) << nl;
                continue;
            }


            // **** pwd command Handler ****
            else if (command == "pwd") {
                vector<string> output = Pwd::get_console_output(args, current_pwd_path);
                
                if (output[0] == "error") {
                    cout << output[1] << nl;
                }
                else {
                    cout << output[0] << nl;
                }
                continue;

            }


            // **** cd Command handler ****
            else if (command == "cd") {
                vector<string> output = Cd::get_console_output(args, home_pwd_path, current_pwd_path, prev_pwd_path, prompt_path_display);
                if (output.size() > 0 && output[0] == "error") {
                    cout << output[1] << nl;
                }
                continue;

            }


            // **** ls command Handler ****
            else if (command == "ls") {
                // cout << "ls args " << args  << nl;
                vector<string> output = Ls::get_console_output(args, current_pwd_path, home_pwd_path);
    
                if (output.size() > 0 && output[0] == "error") {
                    cout << output[1] << nl;
                    continue;
                }
                for (int i = 0; i < output.size(); i++) {
                    cout << output[i];
                }
                cout << nl;
                continue;

            }


            // **** pinfo command handler ****
            else if (command == "pinfo") {
                //Argument Validator
                // if (args.size() > 1) {
                //     cout << "Too many arguments to \"pinfo\" command" << nl;
                //     continue;
                // }
                vector<string> output = Pinfo::get_console_output(args);
                continue;

            }


            // **** search command handler ****
            else if (command == "search") {
                // cout << "Search" << endl;
                vector<bool> output =  Search::get_console_output(args, current_pwd_path);

                for (bool p : output) {
                    cout << (p ? "True\t" : "False\t");
                }
                cout << nl;
                continue;

            }

            // **** history command handler ****
            else if (command == "history") {
                vector<string> output = History::get_console_output(args);

                for (string& p : output) {
                    cout << p << nl;
                }

                continue;
                
            }

            
            // **** foreground process handler ****
            else {
                //Foreground Process
                vector<string> tokenized_string_temp = Util::tokenize_string(input_command, " ");
                // char** c_args = Util::convert_to_c_array(tokenized_string_temp);
                Util::create_foreground_process(tokenized_string_temp);
                // cout << "\"" << command << "\"" << " is not a valid command." << nl;
            }

            cout << endl;
        }


    }




    //Free all dynamically allocated memory 


    end:

    //Close all background processes
    Background::kill_all_bg_processes();


    //Write History to file
    if (!History::write_to_file()) {
        cout << "Unable to write history" << nl;
    }
    else {
        // History written successfully
        // cout << "History Written" << nl;
    }


    return 0;
}