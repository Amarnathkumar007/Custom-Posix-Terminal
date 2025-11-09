
#ifndef pipe_hpp
#define pipe_hpp

#include <string>
#include <vector>

using namespace std;

class Pipe {

    public:
        Pipe();
        ~Pipe();

        //Function Prototypes

        static void parse_command(char *command, char **args);

        static bool run_pipeline(char *commands[], int num_commands);

        static bool get_console_output (string& input_command);



};

#endif