
#ifndef util_hpp
#define util_hpp

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Util {

    public:
        Util();
        ~Util();

        static pid_t foreground_process_id;

        //Function Prototypes

        static char* convert_to_cstr(string& input_str);

        static vector<string> tokenize_string (string& input_str, const char* sep);

        static string get_parent_directory_path (string& current_pwd_path);

        static bool validate_directory_path(string& dir_path);

        static bool validate_file_path(string& file_path);

        static string append_space (string& input_str, int n);

        static int int_len (int num);

        static string get_file_name_from_path(string& final_file_path);

        static bool starts_with(char* c_input_str, const string& prefix);
        
        static bool starts_with_prefix(const string& input_str, string& prefix);

        static int extract_num_from_str(string& input_str);

        static bool sortingPolicy (pair<string, string>& a, pair<string, string>& b);

        static bool is_command_valid (string& command, unordered_map<string, bool>& valid_commands);

        static bool is_num (string& input);

        static string trim_trailing_spaces (string& input_str);

        static void trim_new_line (string& input_str);

        static pair<string, string> get_input_from_raw_terminal();

        static vector<string> get_autocomplete (string& input_string, unordered_map<string, bool>& valid_commands, string& current_pwd_path);

        static bool contains_pipe (string& input_command);

        static string trim_leading_and_trailing_space (string& input_str);

        static char** convert_to_c_array (vector<string>& input_vector);

        static bool contains_background(string& input_string);

        static vector<string> extract_command_from_bg_command (string& input_string);

        static bool create_foreground_process(vector<string>& tokenized_string_temp);

        static bool contains_redirection (string& input_string);





};

#endif