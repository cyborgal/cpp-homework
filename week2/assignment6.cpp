#include <iostream> 
#include <fstream> 
#include <string> 
using namespace std; 
 
class Item { 
public: 
    string name; 
    int quantity; 

    void saveToFile() { 
        ofstream out("items.txt"); 
        if (out.is_open()) { 
            out << name << "," << quantity << endl; 
            out.close(); 
            cout << "Item saved to file." << endl; 
        } else { 
            cout << "Unable to open file for writing." << endl; 
        } 
    }

    void loadFromFile() { 
        ifstream in("items.txt"); 
        if (in.is_open()) { 
            string line; 
            while (getline(in, line)) { 
                cout << "File content: " << line << endl; 
            } 
            in.close(); 
        } else { 
            cout << "Unable to open file for reading." << endl; 
        } 
    } 
}; 
 
int main() { 
    Item tool; 
    tool.name = "Hammer"; 
    tool.quantity = 5; 
 
    tool.saveToFile(); 
    tool.loadFromFile(); // optional 
 
    return 0; 
}
