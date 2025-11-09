#include <iostream>
#include <string>
#include <unistd.h>
#include "system_interface.hpp"

#define nl '\n'

using namespace std;


SystemInterface::SystemInterface() {
    //Constructor Implementation

};

SystemInterface::~SystemInterface() {
    //Destructor Implementation
};


//Function Implementation

string SystemInterface::get_login_user_name (const int& UNAME_BUFF_SIZE) {
    // char username[UNAME_BUFF_SIZE];
    char* username = (char*)malloc(UNAME_BUFF_SIZE * sizeof(char));

    int ret_val = getlogin_r(username, UNAME_BUFF_SIZE);

    if (ret_val != 0) {
        cout << "Error in retrieving user name. The program will now EXIT." << nl;
        exit(EXIT_FAILURE);
    }

    string ret_string = string(username);
    free(username);
    return ret_string;
}


string SystemInterface::get_system_name (const int& SYSNAME_BUFF_SIZE) {
    // char sysname[SYSNAME_BUFF_SIZE];
    char* sysname = (char*)malloc(SYSNAME_BUFF_SIZE * sizeof(char));

    int ret_val = gethostname(sysname, SYSNAME_BUFF_SIZE);

    if (ret_val != 0) {
        cout << "Error in retrieving system name. The program will now EXIT." << nl;
        exit(EXIT_FAILURE);
    }

    // cout << sysname << nl;
    string ret_string = string(sysname);
    free(sysname);
    return ret_string;
}


string SystemInterface::get_pwd (const int& PWD_BUFF_SIZE) {
    char* pwdname = (char*)malloc(PWD_BUFF_SIZE * sizeof(char));

    char* ret_string = getcwd(pwdname, PWD_BUFF_SIZE);

    if (ret_string == nullptr) {
        cout << "Error in retrieving current directory path. The program will now EXIT." << nl;
        exit(EXIT_FAILURE);
    }

    // cout << sysname << nl;
    string return_str = string(pwdname);
    free(pwdname);
    return return_str;

}
