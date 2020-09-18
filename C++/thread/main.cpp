#include "CustomThread.h"

#include <unistd.h>
#include <iostream>

class Test: public CustomThread {

public: 
    Test() {
        a=1;
        b=2;
    }

    ~Test() = default;

    void run () override {
        while (1) {
          std::cout <<"hello world a:"<<a<<" b:"<<b<<std::endl;
          sleep(1);
        }
    } 
private:
   int a;
   int b;

};

int main(int argc, char **argv)
{
    Test a; 
    a.start();
    getchar();

    return 0;
}
