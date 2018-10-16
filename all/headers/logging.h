#pragma once

#include <string>
#include <vector>
#include <time.h>

const int MAX_LOG_MSG_LENGTH = 1000;

class Logging {
public:
    Logging() { index = 0; }

    void logf(char* fmt, ...);
    void printf(char* fmt, ...);
    
    void print_recent();
    void print_all();
    
    void clear();
private:
    void add_to_log(std::string s);
    void print_between(int start, int end);

    std::vector<std::string> log;
    int index;
};