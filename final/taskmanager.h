#pragma once
/*
 * taskmanager.h ― Manages a collection of tasks with file I/O and search capabilities.
 * This class handles a fixed array of up to 10 tasks, providing methods to add, delete,
 * rename, and display tasks. It also supports saving and loading task and session data
 * to/from CSV files, with a binary search for efficient task lookup.
 */

#include "task.h"
#include <string>

class TaskManager {
private:
    Task* tasks[10];  // Fixed-size array to hold task pointers
    int count;        // Number of tasks currently in the array

public:
    TaskManager();                        // Constructor, initializes task count to zero
    ~TaskManager();                       // Destructor, frees all allocated task objects

    void addTask(std::string name);       // Adds a new task if the array limit (10) is not reached
    void showAllTasks();                  // Displays a summary of all tasks to the console
    int binarySearch(std::string target); // Performs binary search to find task by name
    void saveToFile(std::string filename); // Saves all tasks to a specified CSV file
    void loadFromFile(std::string filename); // Loads tasks from a specified CSV file
    void saveSessionsToFile(std::string filename); // Saves all session logs to a specified CSV file
    void loadSessionsFromFile(std::string filename); // Loads session logs from a specified CSV file
    Task* getTaskAt(int index);           // Returns a pointer to the task at the given index
    int getCount() const;                 // Returns the current number of tasks

    void logSession(const std::string& name, long long duration); // Logs a duration to the specified task
    void showSummary() const;             // Displays a summary of all tasks' total durations
    void addDurationToTask(const std::string& name, long long duration); // Adds duration to an existing task

    void deleteTask(int index);                 // Deletes the task at the specified index
    bool renameTask(int index, const std::string& newName); // Renames the task at the specified index
};
