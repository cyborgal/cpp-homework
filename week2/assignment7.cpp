#include <iostream>
#include <string>
using namespace std;

// Base class
class Item {
public:
    string name;
    int quantity;

    Item(string n, int q) {
        name = n;
        quantity = q;
    }

    virtual void display() = 0; // Pure virtual function
};

// Perishable item
class Perishable : public Item {
public:
    Perishable(string n, int q) : Item(n, q) {}

    void display() override {
        cout << name << " (Perishable), Qty: " << quantity << endl;
    }
};

// Non-Perishable item
class NonPerishable : public Item {
public:
    NonPerishable(string n, int q) : Item(n, q) {}

    void display() override {
        cout << name << " (Non-Perishable), Qty: " << quantity << endl;
    }
};

int main() {
    // Create inventory using base class pointers
    Item* item1 = new Perishable("Milk", 10);
    Item* item2 = new NonPerishable("Cereal", 20);

    item1->display();
    item2->display();

    // Clean up
    delete item1;
    delete item2;

    return 0;
}
