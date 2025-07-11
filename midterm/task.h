#pragma once

#include <string>
using namespace std;

class Task {
private:
    string name;           // Task name
    int totalDuration;     // Total time in seconds
    long long startTime;   // Start time in seconds since epoch

public:
    Task();                                // Default constructor
    Task(string taskName);                 // Constructor with name
    Task(string taskName, int duration);   // Constructor with name + duration (for loading)

    void start();                          // Start timer
    void stop();                           // Stop timer
    void display();                        // Show info
    string toCSV();                        // Convert to CSV line
    string getName();                      // Return name
};
