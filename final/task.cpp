#include "task.h"
#include <iostream>
#include <ctime>

/* ── Constructors ───────────────────────────────────────── */

Task::Task() {
    name = "Unnamed";  // Set default task name
    totalDuration = 0; // Initialize total duration to zero
    startTime = 0;     // Initialize start time to indicate not running
}

Task::Task(const std::string& taskName) {
    name = taskName;   // Set the provided task name
    totalDuration = 0; // Initialize total duration to zero
    startTime = 0;     // Initialize start time to indicate not running
}

Task::Task(const std::string& taskName, long long duration) {
    name = taskName;    // Set the provided task name
    totalDuration = duration; // Set initial duration from file load
    startTime = 0;      // Initialize start time to indicate not running
}

/* ── Timer Control ───────────────────────────────────────── */

void Task::start() {
    if (startTime == 0) {  // Check if timer is not already running
        startTime = time(nullptr);  // Set start time to current epoch time
    }
}

void Task::stop() {
    if (startTime == 0) {  // Check if timer was running
        std::cout << "Timer not started.\n";  // Warn user if attempt to stop when not running
        return;
    }
    time_t endTime = time(nullptr);  // Get current time as end time
    long long duration = endTime - startTime;  // Calculate elapsed time
    totalDuration += duration;  // Add elapsed time to total
    sessions.push_back({startTime, endTime, duration});  // Log the session
    startTime = 0;  // Reset start time to indicate stopped
}

void Task::pause() {
    if (startTime != 0) {  // Check if timer is running
        time_t endTime = time(nullptr);  // Get current time as end time
        long long duration = endTime - startTime;  // Calculate elapsed time
        totalDuration += duration;  // Add elapsed time to total
        sessions.push_back({startTime, endTime, duration});  // Log the session
        startTime = 0;  // Reset start time to indicate paused
    }
}

void Task::reset() {
    totalDuration = 0;  // Reset accumulated duration to zero
    startTime = 0;      // Stop the timer
    sessions.clear();   // Clear all logged sessions
}

void Task::addSession(time_t start, time_t end, long long duration) {
    sessions.push_back({start, end, duration});  // Add the provided session to the log
}

/* ── Status & Accessors ──────────────────────────────────── */

bool Task::isRunning() const {
    return startTime != 0;  // Return true if start time is set (timer active)
}

long long Task::getTotalDuration() const {
    if (isRunning()) {
        return totalDuration + (time(nullptr) - startTime);  // Include current session time
    }
    return totalDuration;  // Return total duration if not running
}

long long Task::getLastStartTime() const {
    return startTime;  // Return the last recorded start time
}

const std::vector<Task::Session>& Task::getSessions() const {
    return sessions;  // Return a const reference to the session vector
}

/* ── Metadata ────────────────────────────────────────────── */

void Task::rename(const std::string& newName) {
    name = newName;  // Update the task name
}

std::string Task::getName() const {
    return name;  // Return the current task name
}

/* ── Display & Export ────────────────────────────────────── */

void Task::display() const {
    std::cout << "Task: " << name << ", Total Time: " << totalDuration << " seconds.\n";  // Print task summary
}

std::string Task::toCSV() const {
    return name + "," + std::to_string(totalDuration);  // Return CSV-formatted string
}
