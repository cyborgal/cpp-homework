#include <iostream>
using namespace std;

void showMenu() { 
    cout << "1. Add\n2. View\n3. Exit\n"; 
} 

int main() { 
    int option; 
    do { 
        showMenu();
        cout << "Choose an option: ";
        cin >> option;

        if (option == 1) {
            cout << "Item added!\n"; 
        } 
        else if (option == 2) {
            cout << "Displaying items...\n";
        }

    } while (option != 3);

    cout << "Exiting...\n";
    return 0; 
}
