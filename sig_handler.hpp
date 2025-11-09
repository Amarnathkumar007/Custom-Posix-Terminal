
#ifndef sig_handler_hpp
#define sig_handler_hpp

using namespace std;

class SignalHandler {

    public:
        SignalHandler();
        ~SignalHandler();

        //Function Prototypes

        static void sigint_handler(int dummy_sig);

        static void sigtstp_handler(int dummy_sig);



};

#endif