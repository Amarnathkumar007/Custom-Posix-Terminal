#include <string>
#include <dirent.h>
#include <cstring>
#include "search.hpp"
#include "util.hpp"

#define nl '\n'

using namespace std;


Search::Search() {
    //Constructor Implementation

};

Search::~Search() {
    //Destructor Implementation
};


//Function Implementation

bool Search::find_existence_recursive (string& file_dir_name, string& current_pwd_path) {
    DIR *dir;
    struct dirent* file_entry;

    char* c_current_pwd_path = Util::convert_to_cstr(current_pwd_path);
    char* c_file_dir_name = Util::convert_to_cstr(file_dir_name);

    if ((dir = opendir(c_current_pwd_path)) == NULL) {
        perror("opendir");
        return false;
    }

    while ((file_entry = readdir(dir)) != NULL) {
        //Keep reading the list of files
        if (strcmp(file_entry->d_name, ".") == 0 || strcmp(file_entry->d_name, "..") == 0) {
            continue;
        }

        if (strcmp(file_entry->d_name, c_file_dir_name) == 0) {
            closedir(dir);
            return true;
        }


        if (file_entry->d_type == DT_DIR) {
            string rec_dir_path = current_pwd_path + "/" + string(file_entry->d_name);
            if (find_existence_recursive(file_dir_name, rec_dir_path)) {
                closedir(dir);
                return true;
            }
        }

    }

    closedir(dir);
    return false;
    
}


vector<bool> Search::get_console_output(string& args, string& current_pwd_path) {
    vector<string> tokenized_args = Util::tokenize_string(args, " ");
    vector<bool> console_output;

    for (int i = 0; i < (int)tokenized_args.size(); i++) {
        console_output.push_back(Search::find_existence_recursive(tokenized_args[i], current_pwd_path));
    }

    return console_output;
}

