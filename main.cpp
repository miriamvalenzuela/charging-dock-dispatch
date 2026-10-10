// Name: Miriam Valenzuela
// Red ID: 131786119

#include <string>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <sched.h>

#include "shared.h"
#include "log.h"
#include "dispatcher.h"

using namespace std;

bool readRobotPlans(const string& filePath, vector<Robot>& robots) {
    ifstream inputFile(filePath);

    // Stop if the plan file can't be opened
    if(!inputFile) {
        cout << "Unable to open <<" << filePath << ">>\n";
        return false;
    }

    string line;

    while (getline(inputFile, line)) {
        // Ignore any blank lines from the input file
        if (line.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        istringstream lineStream(line);

        vector<unsigned int> legs;
        int duration;

        // Reads every leg belonging to the current robot
        while (lineStream >> duration) {
            if (duration <= 0) {
                cout << "A leg duration must be bigger than 0.\n";
                return false;
            }

            legs.push_back(static_cast<unsigned int>(duration));
        }

        // Every valid plan should start and end with a charge leg
        if (legs.size() % 2 == 0) {
            cout << "There must be an odd number of legs for each robot.\n";
            return false;
        }

        Robot robot;

        robot.id = static_cast<unsigned int>(robots.size());
        robot.legs = legs;
        robot.remainingLegMinutes = legs[0];

        robots.push_back(robot);
    }

    return true;
}

void printRobotPlans(vector<Robot>& robots) {
    for (Robot& robot: robots) {
        log_robot_legs(robot.legs.data(), robot.legs.size());
    }
}

int main (int argc, char* argv[]) {
    DispatchPolicy policy = DispatchPolicy::FCFS;
    unsigned int quantum = 2;

    int option; 

    while ((option = getopt(argc, argv, "s:q:")) != -1) {
        switch (option) {
            case 's': {
                string strategy = optarg;

                if (strategy == "rr") {
                    policy = DispatchPolicy::RR;
                }else {
                    policy = DispatchPolicy::FCFS;
                }

                break;
            }

            case 'q':
                quantum = static_cast<unsigned int>(atoi(optarg));
                break;

            default:
                break;
        }
    }

    if (optind >= argc) {
        return 0;
    }

    string filePath = argv[optind];

    SharedData sharedData;

    sharedData.policy = policy;
    sharedData.quantum = quantum;

    if (!readRobotPlans(filePath, sharedData.robots)) {
        return 0;
    }

    printRobotPlans(sharedData.robots);

    pthread_t dispatcher;

    // Starts the worker thread which will run the dispatch simulation
    if (pthread_create(&dispatcher, nullptr, dispatcherThread, &sharedData) != 0) {
        cout << "Unable to create dispatcher thread\n";
        return 0;
    }

    // Busy wait until the dispatcher indicates that the simulation has finished
    while (!sharedData.dispatcherDone.load()) {
        sched_yield();
    }

    return 0;

}