#include <iostream>
#include <fstream>
#include "taskmanager.h"
using namespace std;

// Constructor
TaskManager::TaskManager() {
    count = 0;
}

// Destructor
TaskManager::~TaskManager() {
    for (int i = 0; i < count; i++) {
        delete tasks[i];
    }
}

// Add a task
void TaskManager::addTask(string name) {
    if (count >= 10) {
        cout << "Task limit reached!\n";
        return;
    }
    tasks[count] = new Task(name);
    count++;
}

// Show all tasks
void TaskManager::showAllTasks() {
    for (int i = 0; i < count; i++) {
        tasks[i]->display();
    }
}

// Binary search
int TaskManager::binarySearch(string target) {
    int left = 0;
    int right = count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        string midName = tasks[mid]->getName();
        if (midName == target)
            return mid;
        if (midName < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

// Save to file
void TaskManager::saveToFile(string filename) {
    ofstream outFile(filename);
    for (int i = 0; i < count; i++) {
        outFile << tasks[i]->toCSV() << endl;
    }
    outFile.close();
}

// Load from file
void TaskManager::loadFromFile(string filename) {
    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string name = line.substr(0, commaPos);
            int duration = stoi(line.substr(commaPos + 1));
            if (count < 10) {
                tasks[count] = new Task(name, duration);
                count++;
            }
        }
    }
    inFile.close();
}

// Get pointer to task
Task* TaskManager::getTaskAt(int index) {
    if (index >= 0 && index < count)
        return tasks[index];
    return nullptr;
}
