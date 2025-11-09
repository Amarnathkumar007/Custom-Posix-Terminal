
#ifndef search_hpp
#define search_hpp

#include <string>
#include <vector>

using namespace std;

class Search {

    public:
        Search();
        ~Search();

        //Function Prototypes

        static vector<bool> get_console_output (string& args, string& current_pwd_path);

        static bool find_existence_recursive (string& file_dir_name, string& current_pwd_path);



};

#endif