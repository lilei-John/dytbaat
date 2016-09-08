//
// Created by Rasmus Haugaard on 08/09/2016.
//

#include "Dog.h"

string Dog::bark() {
    return _name + " barks: " + _barkSound;
}

Dog::Dog(string name, string barkSound) {
    _name = name;
    _barkSound = barkSound;
}
