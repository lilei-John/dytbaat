//
// Created by Haukur Kristinsson on 13/09/16.
//

#include "Cat.h"

Cat::Cat() {

}

Cat::Cat(string name, string newMeowSound) {
    catName = name;
    meowSound = newMeowSound;
}

string Cat::meow() {
    return catName + " meows: " + meowSound;
}