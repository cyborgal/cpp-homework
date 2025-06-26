#include <iostream>
#include <string>
using namespace std;

struct Item {
    string name;
    int id;
};

string binarySearch(Item* items, int targetId, int low, int high) {
    if (low > high) {
        return "Not found"; // Not found
    }

    int mid = (low + high) / 2;

    if (items[mid].id == targetId) {
        return items[mid].name;
    } else if (items[mid].id < targetId) {
        // Search right half
        return binarySearch(items, targetId, mid + 1, high);
    } else {
        // Search left half
        return binarySearch(items, targetId, low, mid - 1);
    }
}


int main() {

    Item* inventory = new Item[5];

    inventory[0] = {"Sword", 1};
    inventory[1] = {"Shield", 2};
    inventory[2] = {"Potion", 3};
    inventory[3] = {"Bow", 4};
    inventory[4] = {"Arrow", 5};

    int targetId;
    cout << "Enter item ID to search: ";
    cin >> targetId;

    string result = binarySearch(inventory, targetId, 0, 4);

    cout << result << endl;

    delete[] inventory;
}
