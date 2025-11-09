
#ifndef redirect_hpp
#define redirect_hpp


using namespace std;

class Redirect {

    public:
        Redirect();
        ~Redirect();


        //Function Prototypes

        static void in_redirection (char *fileName);
        
        static void out_redirection (char *fileName);

        static void run_redirection(char *arguments[]);

        static string string_tokenize (string & input_command);

        static bool get_console_output (string& input_command);



};

#endif