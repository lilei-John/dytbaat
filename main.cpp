#include <iostream>
#include "Dog.h"

using namespace std;

int main() {

    string name, barkSound;

    cout << "What is the name of your dog? ";
    cin >> name;
    cout << "How does your dog bark? ";
    cin >> barkSound;

    Dog myDawg(name, barkSound);
    cout << myDawg.bark() << endl;

    return 0;
}