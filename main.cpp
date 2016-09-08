#include <iostream>
#include "Dog.h"

using namespace std;

int main() {
    Dog myDawg("King", "VOOF!");
    cout << myDawg.bark() << endl;
    return 0;
}