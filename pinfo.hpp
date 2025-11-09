
#ifndef pinfo_hpp
#define pinfo_hpp

#include <string>
#include <vector>

using namespace std;

class Pinfo {

    public:
        Pinfo();
        ~Pinfo();

        //Function Prototypes
        
        static vector<string> get_console_output(string& args);



};

#endif