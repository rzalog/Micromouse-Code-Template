#pragma once

/***
 * Assignment 2
 * 
 * Experimentally determine this value.
 ***/
const float COUNTS_FOR_CELL = 0; // TODO

class Encoder {
public:
    Encoder();
    void reset() volatile;
    void update() volatile;
    int getPulsesR() volatile;
    int getPulsesL() volatile;
    
    void printValues() volatile;
private:    
    volatile int m_countR;
    volatile int m_countL;
};
