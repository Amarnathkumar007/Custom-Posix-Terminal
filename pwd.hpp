
#ifndef pwd_hpp
#define pwd_hpp

#include <string>
#include <vector>

using namespace std;

class Pwd {

    public:
        Pwd();
        ~Pwd();


        //Function Prototypes

        static vector<string> get_console_output (string& args, string& current_pwd_path);






};

#endif