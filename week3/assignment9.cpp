#include <iostream>
#include <string>
using namespace std;

class Item {
public:
    string name;
    int quantity;

    void display() {
        cout << "Item: " << name << ", Quantity: " << quantity << endl;
    }
};

void searchItem(Item inventory[], int size, string searchName) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (inventory[i].name == searchName) {
            cout << "Found: " << inventory[i].name << " - Quantity: " << inventory[i].quantity << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Item not found in inventory." << endl;
    }
}

int main() {
    const int SIZE = 5;
    Item inventory[SIZE];

    // Input items
    for (int i = 0; i < SIZE; i++) {
        cout << "Enter name for item " << (i + 1) << ": ";
        cin >> inventory[i].name;
        cout << "Enter quantity for " << inventory[i].name << ": ";
        cin >> inventory[i].quantity;
    }

    // Display items
    cout << "\nInventory List:\n";
    for (int i = 0; i < SIZE; i++) {
        inventory[i].display();
    }

    // Search for an item
    string searchName;
    cout << "\nEnter item name to search: ";
    cin >> searchName;
    searchItem(inventory, SIZE, searchName);

    return 0;
}

