#include "Point.h"
#include <iostream>
using  namespace std;

Point::Point(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Point::setPoint(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Point::printPoint()
{
    cout << "x="<< m_x <<" "<<"y="<< m_y << endl;
}

int Point::getX()
{
    return m_x;
}

int Point::getY()
{
    return m_y;
}
