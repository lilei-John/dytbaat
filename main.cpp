#include <iostream>
#include "Dog.h"
#include "Cat.h"

using namespace std;

int main() {

    string name, Sound, catOrDog;
    cout << "Do you own a cat or a dog? ";
    cin >> catOrDog;
    if (catOrDog == "Dog" || catOrDog == "dog") {
        cout << "What is the name of your dog? ";
        cin >> name;
        cout << "How does your dog bark? ";
        cin >> Sound;
        Dog myDawg(name, Sound);
        cout << myDawg.bark() << endl;
    }
    else if (catOrDog == "Cat" || catOrDog == "cat") {
        cout << "What is the name of your cat? ";
        cin >> name;
        cout << "How does your cat meow? ";
        cin >> Sound;
        Cat myCat(name, Sound);
        cout << myCat.meow() << endl;

    }

    return 0;
}