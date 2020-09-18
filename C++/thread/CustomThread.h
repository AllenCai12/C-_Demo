#include <thread>

class CustomThread {

public:
        void start(){ 
            std::thread t(std::bind(&CustomThread::run,this));
            t.detach();
        }

        virtual void run() = 0;

};


