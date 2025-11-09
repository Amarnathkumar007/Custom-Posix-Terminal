
#ifndef system_hpp
#define system_hpp

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SystemInterface {

    public:
        SystemInterface();
        ~SystemInterface();

        //Function Prototypes

        static string get_login_user_name (const int& UNAME_BUFF_SIZE);

        static string get_system_name (const int& SYSNAME_BUFF_SIZE);

        static string get_pwd (const int& PWD_BUFF_SIZE);



};

#endif