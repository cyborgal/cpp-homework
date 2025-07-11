#include <iostream>
#include <ctime>
#include "task.h"
using namespace std;

// Default constructor
Task::Task() {
    name = "Unnamed";
    totalDuration = 0;
    startTime = 0;
}

// Constructor with name
Task::Task(string taskName) {
    name = taskName;
    totalDuration = 0;
    startTime = 0;
}

// Constructor with name + duration
Task::Task(string taskName, int duration) {
    name = taskName;
    totalDuration = duration;
    startTime = 0;
}

// Start timer
void Task::start() {
    startTime = time(nullptr);
}

// Stop timer
void Task::stop() {
    if (startTime == 0) {
        cout << "Timer not started.\n";
        return;
    }
    long long endTime = time(nullptr);
    totalDuration += (endTime - startTime);
    startTime = 0;
}

// Show info
void Task::display() {
    cout << "Task: " << name << ", Total Time: " << totalDuration << " seconds.\n";
}

// Convert to CSV
string Task::toCSV() {
    return name + "," + to_string(totalDuration);
}

// Get name
string Task::getName() {
    return name;
}
