#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "sig_handler.hpp"
#include "util.hpp"
#include "background.hpp"

#define nl '\n'

using namespace std;


SignalHandler::SignalHandler() {
    //Constructor Implementation
};

SignalHandler::~SignalHandler() {
    //Destructor Implementation
};


//Function Implementation

void SignalHandler::sigint_handler(int dummy_sig) {
    pid_t process_id = Util::foreground_process_id;
    if (process_id == 0) {
        cout << "No foreground processes running" << endl;
        return;
    }
    else {
        cout << "Sending SIGINT signal to foreground processes" << nl;
        //Interrupt any currently running foreground job
        kill(process_id, SIGINT);
        // exit(EXIT_SUCCESS);
        return;
    }
}


void SignalHandler::sigtstp_handler(int dummy_sig) {
    pid_t process_id = Util::foreground_process_id;
    pid_t group_id = getpgrp();

    int return_val = 0;
    //Push foreground job to background and change state from running to stopped
     if (process_id == 0) {
        cout << "No foreground processes running" << endl;
        return;
    }
    else {
        cout << "Sending SIGSTOP signal to foreground processes" << nl;
        //Interrupt any currently running foreground job
        if (kill(process_id, SIGSTOP) == -1) {
            cout << "Failed to stop Foreground process" << endl;
            exit(EXIT_FAILURE);
            return;
        }
        if (kill(process_id, SIGCONT) == -1) {
            cout << "Failed to continue process" << endl;
            exit(EXIT_FAILURE);
            return;
        }

        cout << "Pushed to background" << endl;
        Background::process_list.push_back(process_id);
        waitpid(process_id, &return_val, WNOHANG);
        // goto mainloop;
        // exit(EXIT_SUCCESS);
        return;
    }
    // exit(EXIT_SUCCESS);
}


