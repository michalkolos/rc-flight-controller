//
// Created by kolos on 21.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_WORKERTHREAD_H
#define RC_FLIGHT_CONTROLLER_WORKERTHREAD_H


#include <functional>

class WorkerThread {
private:

public:
    void start();
    void executeTask(const std::function<void()>& functionPtr);

    
};


#endif //RC_FLIGHT_CONTROLLER_WORKERTHREAD_H
