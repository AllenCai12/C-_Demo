#ifndef CPP_PORT_H
#define CPP_PORT_H

class Point {
private:
    int m_x;
    int m_y;

public:
    Point(int x, int y);
    void setPoint(int x, int y);
    void printPoint();
    int getX();
    int getY();
};

#endif

