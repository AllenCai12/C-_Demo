#include "Line.h"
#include <iostream>

using  namespace std;

Line::Line(int l): Point(0, l)
{
    cout<<"construct line"<<endl;
    m_length = l;
}

void Line::setLength(int length)
{
    m_length = length;
}

int Line::getLength()
{
    return m_length;
}


