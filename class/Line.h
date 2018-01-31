#ifndef CPP_LINE_H
#define CPP_LINE_H

#include <iostream>
using namespace std;

class Line {

private:
    int m_length;   
        
public:
    Line(int length);
    void setLength(int length);
    int getLength();
};

#endif
