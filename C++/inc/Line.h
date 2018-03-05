#ifndef CPP_LINE_H
#define CPP_LINE_H

#include <iostream>
#include "Point.h"

using namespace std;

class Line: public Point {

private:
    int m_length;   
        
public:
    Line();
    void setLength(int length);
    int getLength();
};

#endif
