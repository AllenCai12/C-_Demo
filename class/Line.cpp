#include "Line.h"

Line::Line(int length) 
{
    m_length = length;
}

void Line::setLength(int length)
{
    m_length = length;
}

int Line::getLength()
{
    return m_length;
}


