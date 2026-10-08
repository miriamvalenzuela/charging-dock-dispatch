// Name: Miriam Valenzuela
// Red ID: 131786119

#ifndef SHARED_H
#define SHARED_H

#include <atomic>
#include <cstddef>
#include <vector>


// Determines which dock scheduling policy the dispatcher should use
enum class DispatchPolicy {
    FCFS,  // First Come First Serve
    RR     // Round Robin
};


// Stores the plan and current simulation state for a robot
struct Robot {
    unsigned int id;
    std::vector<unsigned int> legs;

    std::size_t legIndex = 0; 
    unsigned int remainingLegMinutes = 0; 

    unsigned int chargedMinutes = 0;
    unsigned int deliveryMinutes = 0; 

    unsigned int shiftTime = 0; 
};


// Contains all of thhe shared information between main and the dispatcher thread
struct SharedData {
    std::vector<Robot> robots;

    DispatchPolicy policy = DispatchPolicy::FCFS;
    unsigned int quantum = 0; //Time quantum

    std::atomic<bool> dispatcherDone{false};
};


#endif