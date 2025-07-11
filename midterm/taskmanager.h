#pragma once

#include "task.h"

class TaskManager {
private:
    Task* tasks[10];   // Array of pointers to Tasks
    int count;         // Number of tasks

public:
    TaskManager();                       // Constructor
    ~TaskManager();                      // Destructor
    void addTask(string name);           // Add new task
    void showAllTasks();                 // Display all tasks
    int binarySearch(string target);     // Search by name
    void saveToFile(string filename);    // Save all tasks
    void loadFromFile(string filename);  // Load tasks
    Task* getTaskAt(int index);          // Get pointer to task
};
