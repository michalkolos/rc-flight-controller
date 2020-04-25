#include <iostream>
#include <thread>
#include <unistd.h>
#include "Sbus.h"
#include "Servos.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

//    Sbus sbus("/dev/ttyS0");
//
    Servos servos("/dev/i2c-1",  0x40, 50);
    servos.initialize();
    servos.addServo(0, 200, 400);
    servos.addServo(1, 200, 400);
    servos.addServo(2, 200, 400);

    servos.moveServo(2,1000);
    usleep(20000000);

    servos.disableInterface();
//    servos.test();


    std::mutex threadMutex;


//    std::thread t1([&sbus](){
//        sbus.serialBegin();
//        while (true) {
//            sbus.checkSbus();
//        }
//    });
//
//    std::thread t2([&sbus](){
//        while (true) {
//            if(sbus.getFreshness() == 0){
//                for(auto channel: sbus.getChannels()){
//                    std::cout << channel << " ";
//                }
//                std::cout << std::endl;
//            }
//        }
//    });
//
//    t1.join();
//    t2.join();

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

//    std::cout << "finish \n";
    return 0;
}
