#ifndef _LINE_H_
#define _LINE_H_

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
