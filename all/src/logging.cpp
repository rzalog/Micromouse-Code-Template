#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"
#include "globals.h"

using namespace std;

void Logging::logf(char* fmt, ...) {
    char buffer[MAX_LOG_MSG_LENGTH];
   
    va_list aptr;

    va_start(aptr, fmt);
    vsprintf(buffer, fmt, aptr);
    va_end(aptr);
    
    string to_add(buffer);
    
    this->add_to_log(to_add);
}

void Logging::printf(char* fmt, ...) {
    char buffer[MAX_LOG_MSG_LENGTH];
   
    va_list aptr;

    va_start(aptr, fmt);
    vsprintf(buffer, fmt, aptr);
    va_end(aptr);
    
    pc.printf("%s\n", buffer);
    
    string to_add(buffer);
    
    this->add_to_log(to_add);
}

void Logging::add_to_log(string s) {
    this->log.push_back(s);
}

void Logging::print_recent() {
    this->print_between(index, log.size());
    index = log.size();
}   

void Logging::print_all() {
    this->print_between(0, log.size());
}

void Logging::print_between(int start, int end) {
    for (int i=0; i < log.size() && i < end; i++) {
        pc.printf("%s\n", log[i].c_str());
    }
}

void Logging::clear() {
    this->log.clear();
    index = 0;
}