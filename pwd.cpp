#include <iostream>
#include "pwd.hpp"
#include "util.hpp"

#define nl '\n'

using namespace std;


Pwd::Pwd() {
    //Constructor Implementation
};

Pwd::~Pwd() {
    //Destructor Implementation
};


//Function Implementation

vector<string> Pwd::get_console_output (string& args, string& current_pwd_path) {
    vector<string> tokenized_args = Util::tokenize_string(args, " ");
    vector<string> output; 

    if (tokenized_args.size() > 0) {
        output.push_back("error");
        output.push_back("Too many arguments to \"pwd\" command");
        return output;
    }
    else {
        output.push_back(current_pwd_path);
    }

    return output;

}

// void SignalHandler::sigint_handler(int dummy_sig) {
//     cout << "SIGINT signal received...HANDLED" << nl;
//     //Interrupt any currently running foreground job
//     exit(EXIT_SUCCESS);

// }


