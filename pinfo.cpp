#include <iostream>
#include <string>
#include <unistd.h>
#include "pinfo.hpp"
#include "util.hpp"

#define nl '\n'

using namespace std;


Pinfo::Pinfo() {
    //Constructor Implementation

};

Pinfo::~Pinfo() {
    //Destructor Implementation
};




//Function Implementation

vector<string> Pinfo::get_console_output(string& args) {
    vector<string> output;
    pid_t current_process_pid = 0;

    string path_to_process_data = "";
    string path_to_process_exec = "";

    if (args.size() == 0) {
        current_process_pid = getpid();
        path_to_process_data = "/proc/self/status";
        path_to_process_exec = "/proc/self/exe";
    }
    else {
        current_process_pid = stoi(args);
        path_to_process_data = "/proc/" + string(args) + "/status";
        path_to_process_exec = "/proc/" + string(args) + "/exe";
    }
    output.push_back("pid -- " + to_string(current_process_pid));

    char* c_path_to_process_data = Util::convert_to_cstr(path_to_process_data);
    char* c_path_to_process_exec = Util::convert_to_cstr(path_to_process_exec);

    // cout << c_path_to_process_data << " " << c_path_to_process_exec << nl;
    
    FILE *file = fopen(c_path_to_process_data, "r");
    if (file == NULL) {
        perror("fopen");
        // return 1;
        // cout << "No such process exists" << nl;
        return output;
    }

    char line[256];
    int mem_use = 0;
    while (fgets(line, sizeof(line), file)) {
        if (Util::starts_with(line, "State")) {
            string temp = string(line).substr(7);
            temp = temp.substr(0, temp.size() - 1);
            output.push_back("Process Status -- " + temp);
        // break;
        }
        else if (Util::starts_with(line, "VmHWM")) {
            string mem_data = string(line);
            mem_use += Util::extract_num_from_str(mem_data);
        }
        else if (Util::starts_with(line, "VmRSS")) {
            string mem_data = string(line);
            mem_use += Util::extract_num_from_str(mem_data);
        }
        // printf("%s", line);
    }

    //Memory Logic
    string memory_use = to_string(mem_use);


    output.push_back("memory -- " + memory_use + " {Virtual Memory}");

    //Executable path
    const int EXEC_PATH_LEN = 200;
    char executable_path[EXEC_PATH_LEN];
    int bytes_written = 0;
    if ((bytes_written = readlink(c_path_to_process_exec, executable_path, EXEC_PATH_LEN)) == -1) {
        cout << "Unable to read executable path" << nl; 
    }
    else {
        executable_path[bytes_written] = '\0';
        output.push_back("Executable Path --" + string(executable_path));
    }


    cout << output[0] << nl;
    cout << output[1] << nl;
    cout << output[2] << nl;
    cout << output[3] << nl;

    fclose(file);



return output;



}

