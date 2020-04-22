#include <iostream>
#include <thread>
#include "Sbus.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Sbus sbus("/dev/ttyS0");

    std::mutex threadMutex;


    std::thread t1([&sbus](){
        while (true) {
            sbus.checkSbus();
        }
    });

    std::thread t2([&sbus](){
        while (true) {
            for(auto channel: sbus.getChannels()){
                std::cout << channel << " ";
            }
            std::cout << std::endl;
        }
    });

    t1.join();
    t2.join();

//    std::thread sbusInThread([](){
//        sbus.checkSbus();
//    });
//
//
//
//    while(true){
//        auto channels = sbus.getChannels();
//        for(auto channel: channels){
//            std::cout <<  channel << " " ;
//        }
//        std::cout << std::endl;
//
//    }

    std::cout << "finish \n";
    return 0;
}
