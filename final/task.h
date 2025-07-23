#pragma once
/*
 * task.h ― Represents a single user task with timing controls and session logging.
 * This class encapsulates a task's name, total duration, current start time, and a
 * history of session logs. It provides methods to manage task timing (start, stop,
 * pause, reset), retrieve status, modify metadata, and serialize data for storage.
 * The Session struct within the class tracks individual work sessions with start
 * time, end time, and duration.
 */

#include <string>
#include <vector>

class Task {
public:
    struct Session {
        time_t startTime;  // Start time of the session in epoch seconds
        time_t endTime;    // End time of the session in epoch seconds
        long long duration; // Duration of the session in seconds
    };

private:
    std::string name;          // Human-readable name of the task
    long long totalDuration;   // Total accumulated time in seconds across all sessions
    long long startTime;       // Last start time in epoch seconds (0 if not running)
    std::vector<Session> sessions; // Vector storing all logged sessions for the task

public:
    /* ── Constructors ───────────────────────────────────────── */
    Task();                                            // Default constructor, initializes task with "Unnamed" name
    Task(const std::string& taskName);                 // Constructor with a specified task name
    Task(const std::string& taskName, long long dur);  // Constructor for loading from file with initial duration

    /* ── Timer Control ──────────────────────────────────────── */
    void start();   // Starts or resumes the task timer, setting startTime to current time
    void stop();    // Stops the timer, adds elapsed time to totalDuration, and logs a session
    void pause();   // Pauses the timer (similar to stop but without warnings if not running)
    void reset();   // Resets total duration to zero and clears all session logs
    void addSession(time_t start, time_t end, long long duration); // Adds a pre-calculated session to the log

    /* ── Quick Status Helpers ───────────────────────────────── */
    bool isRunning() const;              // Returns true if the task timer is currently active
    long long getTotalDuration() const;  // Returns the total duration, including current session if running
    long long getLastStartTime() const;  // Returns the last start time (0 if timer is stopped)
    const std::vector<Session>& getSessions() const; // Returns a const reference to the session vector

    /* ── Metadata Helpers ───────────────────────────────────── */
    void rename(const std::string& newName); // Updates the task name to a new value
    std::string getName() const;             // Returns the current task name

    /* ── I/O Helpers ────────────────────────────────────────── */
    void display() const;      // Prints a summary of the task (name and total duration) to console
    std::string toCSV() const; // Serializes the task data into a CSV string (name,duration)
};
