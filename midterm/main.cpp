#include <iostream>
#include "taskmanager.h"
using namespace std;

int main() {
    TaskManager manager;
    manager.loadFromFile("tasks.csv");

    int choice;
    do {
        cout << "\n--- Task Timer Menu ---\n";
        cout << "1. Add Task\n";
        cout << "2. Show All Tasks\n";
        cout << "3. Start Task Timer\n";
        cout << "4. Stop Task Timer\n";
        cout << "5. Save and Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name;
            cout << "Enter task name: ";
            getline(cin, name);
            manager.addTask(name);

        } else if (choice == 2) {
            manager.showAllTasks();

        } else if (choice == 3) {
            string target;
            cout << "Enter task name to start timer: ";
            getline(cin, target);
            int idx = manager.binarySearch(target);
            if (idx != -1) {
                manager.getTaskAt(idx)->start();
                cout << "Timer started.\n";
            } else {
                cout << "Task not found.\n";
            }

        } else if (choice == 4) {
            string target;
            cout << "Enter task name to stop timer: ";
            getline(cin, target);
            int idx = manager.binarySearch(target);
            if (idx != -1) {
                manager.getTaskAt(idx)->stop();
                cout << "Timer stopped.\n";
            } else {
                cout << "Task not found.\n";
            }
        }

    } while (choice != 5);

    manager.saveToFile("tasks.csv");
    cout << "Tasks saved. Goodbye!\n";
    return 0;
}
