#include <iostream>
#include "Sbus.h"

int main() {
    std::cout << "Hello, World!!!!" << std::endl;

    Sbus sbus("/dev/ttyS0");

    while(true){
        for(auto channel: sbus.getChannels()){
            std::cout <<  channel << " " ;
        }
        std::cout << std::endl;
    }

    std::cout << "finish \n";
    return 0;
}
