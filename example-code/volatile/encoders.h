#pragma once

class Encoders {
public:
    void update() volatile;
    int right() volatile;
    int left() volatile;
private:
    // member variables volatile as long as class is declared volatile
    int right;
    int left;
}