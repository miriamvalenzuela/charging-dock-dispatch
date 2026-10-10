// Name: Miriam Valenzuela
// Red ID: 131786119

#include "dispatcher.h"

#include <deque>
#include <vector>
#include <algorithm>

#include "log.h"
#include "shared.h"

using namespace std; 

// Inserts a robot while keeping the road queue stable-sorted by return time
void insertRoadStable(Robot* robot, vector<Robot*>& roadQueue) {
    auto position = roadQueue.begin(); 

    // Existing robots stay ahead when two return times are equal
    while (position != roadQueue.end() && (*position)->remainingLegMinutes <= robot->remainingLegMinutes) {
        ++position;
    }

    roadQueue.insert(position, robot); 
}

// Advances every robot on the road and moves the completed deliveries to the dock queue
void advanceRoad(unsigned int minutes, vector<Robot*>& roadQueue, deque<Robot*>& dockQueue) {
    for (Robot* robot : roadQueue) {
        unsigned int progressed = min(minutes, robot->remainingLegMinutes);

        robot->remainingLegMinutes -= progressed;
        robot->deliveryMinutes += progressed;
    }

    // Completed runs appear first because the road queue is sorted
    while (!roadQueue.empty() && roadQueue.front()->remainingLegMinutes == 0) {
        Robot* robot = roadQueue.front();

        roadQueue.erase(roadQueue.begin());

        // Moves from the completed delivery leg to the next charge leg
        ++robot->legIndex;

        robot->remainingLegMinutes = robot->legs[robot->legIndex];

        dockQueue.push_back(robot);
    }
}

void runSimulation(SharedData& sharedData) {
    deque<Robot*> dockQueue;
    vector<Robot*> roadQueue; 
    vector<Robot*> finishedRobots;

    unsigned int elapsedTime = 0; 

    // All robots are admitted to the depot using input file order
    for (Robot& robot : sharedData.robots) {
        dockQueue.push_back(&robot);
    }

    while (finishedRobots.size() < sharedData.robots.size()) {
        // If no robot is queued advance the time until the next delivery is finished
        if (dockQueue.empty()) {
            unsigned int idleTime = roadQueue.front()->remainingLegMinutes;

            advanceRoad(idleTime, roadQueue, dockQueue);

            elapsedTime += idleTime;
            continue;
        }

        Robot* current = dockQueue.front();
        dockQueue.pop_front(); 

        unsigned int serviceMinutes = current->remainingLegMinutes;

        advanceRoad(serviceMinutes, roadQueue, dockQueue); 

        elapsedTime += serviceMinutes; 

        current->chargedMinutes += serviceMinutes;
        current->remainingLegMinutes = 0; 

        // The final charge leg completes the robot's shift
        if (current->legIndex == current->legs.size() - 1) {
            current->shiftTime = elapsedTime; 

            log_charge_leg(current->id, current->chargedMinutes, current->deliveryMinutes, elapsedTime, SHIFT_COMPLETE);

            finishedRobots.push_back(current);
            continue; 
        }

        log_charge_leg(current->id, current->chargedMinutes, current->deliveryMinutes, elapsedTime, DEPART_DELIVERY);

        // Move from the completed charge leg to the next delivery leg
        ++current->legIndex;

        current->remainingLegMinutes = current->legs[current->legIndex];

        insertRoadStable(current, roadQueue);
    }

    // Print final robot stats in completion order
    for (Robot* robot : finishedRobots) {
        unsigned int queueTime = robot->shiftTime - robot->chargedMinutes - robot->deliveryMinutes;

        log_robot_completion(robot->id, robot->shiftTime, queueTime);
    }
}

// Runs the simulation inside the dispatcher worker thread
void* dispatcherThread(void* argument) {
    SharedData* sharedData = static_cast<SharedData*>(argument);

    runSimulation(*sharedData);

    // Signal the main thread tht the simulation has finished
    sharedData->dispatcherDone.store(true);

    return nullptr;
}



