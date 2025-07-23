#include "taskmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>

TaskManager::TaskManager() {
    count = 0;  // Initialize the task count to zero
}

TaskManager::~TaskManager() {
    for (int i = 0; i < count; i++) {
        delete tasks[i];  // Deallocate each task object to prevent memory leaks
    }
}

void TaskManager::addTask(std::string name) {
    if (count >= 10) {  // Check if the maximum number of tasks (10) is reached
        std::cout << "Task limit reached!\n";
        return;
    }
    tasks[count] = new Task(name);  // Create a new task and add it to the array
    count++;
}

void TaskManager::showAllTasks() {
    for (int i = 0; i < count; i++) {
        tasks[i]->display();  // Call display method for each task
    }
}

int TaskManager::binarySearch(std::string target) {
    int left = 0, right = count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        std::string midName = tasks[mid]->getName();
        if (midName == target) return mid;  // Return index if found
        if (midName < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;  // Return -1 if target not found
}

void TaskManager::saveToFile(std::string filename) {
    std::ofstream outFile(filename);  // Open file for writing
    for (int i = 0; i < count; i++) {
        outFile << tasks[i]->toCSV() << std::endl;  // Write each task's CSV data
    }
    outFile.close();  // Close the file
}

void TaskManager::loadFromFile(std::string filename) {
    std::ifstream inFile(filename);  // Open file for reading
    std::string line;
    while (getline(inFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string name = line.substr(0, commaPos);
            long long duration = std::stoll(line.substr(commaPos + 1));
            if (count < 10) {
                tasks[count] = new Task(name, duration);  // Create task from file data
                count++;
            }
        }
    }
    inFile.close();  // Close the file
}

void TaskManager::saveSessionsToFile(std::string filename) {
    std::ofstream outFile(filename);  // Open file for writing
    for (int i = 0; i < count; i++) {
        Task* t = tasks[i];
        const auto& sessions = t->getSessions();
        for (const auto& session : sessions) {
            outFile << t->getName() << ","  // Write task name
                    << session.startTime << ","  // Write start time
                    << session.endTime << ","    // Write end time
                    << session.duration << std::endl;  // Write duration
        }
    }
    outFile.close();  // Close the file
}

void TaskManager::loadSessionsFromFile(std::string filename) {
    std::ifstream inFile(filename);  // Open file for reading
    std::string line;
    while (getline(inFile, line)) {
        std::stringstream ss(line);  // Parse line into stream
        std::string name;
        time_t startTime, endTime;
        long long duration;
        std::string token;
        if (getline(ss, name, ',') &&  // Extract name
            getline(ss, token, ',') && (startTime = std::stoll(token)) &&  // Extract start time
            getline(ss, token, ',') && (endTime = std::stoll(token)) &&    // Extract end time
            getline(ss, token) && (duration = std::stoll(token))) {
            int index = binarySearch(name);  // Find task index
            if (index != -1) {
                tasks[index]->addSession(startTime, endTime, duration);  // Add session to task
            }
        }
    }
    inFile.close();  // Close the file
}

Task* TaskManager::getTaskAt(int index) {
    if (index >= 0 && index < count) return tasks[index];  // Return task if index is valid
    return nullptr;  // Return null if index is out of bounds
}

int TaskManager::getCount() const {
    return count;  // Return the current number of tasks
}

void TaskManager::logSession(const std::string& name, long long duration) {
    addDurationToTask(name, duration);  // Delegate to addDurationToTask
}

void TaskManager::addDurationToTask(const std::string& name, long long duration) {
    int index = binarySearch(name);  // Find task index
    if (index != -1) {
        Task* t = tasks[index];
        long long total = t->getTotalDuration();
        delete t;  // Free old task object
        tasks[index] = new Task(name, total + duration);  // Create new task with updated duration
    }
}

void TaskManager::showSummary() const {
    std::cout << "\n--- Summary ---\n";
    for (int i = 0; i < count; i++) {
        std::cout << tasks[i]->getName() << ": "  // Print task name
                  << tasks[i]->getTotalDuration() << "s\n";  // Print total duration
    }
}

void TaskManager::deleteTask(int index) {
    if (index >= 0 && index < count) {
        delete tasks[index];  // Free the task object
        for (int i = index; i < count - 1; ++i) {
            tasks[i] = tasks[i + 1];  // Shift remaining tasks
        }
        count--;  // Decrease task count
        saveToFile("tasks.csv");  // Save updated task list
        saveSessionsToFile("sessions.csv");  // Save updated session list
    }
}

bool TaskManager::renameTask(int index, const std::string& newName) {
    if (index >= 0 && index < count) {
        tasks[index]->rename(newName);  // Update task name
        saveToFile("tasks.csv");  // Save updated task list
        saveSessionsToFile("sessions.csv");  // Save updated session list
        return true;  // Indicate success
    }
    return false;  // Indicate failure
}
