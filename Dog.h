//
// Created by Rasmus Haugaard on 08/09/2016.
//

#pragma once
#include <string>
using namespace std;

class Dog {
public:
    Dog(string name, string barkSound);
    string bark();
private:
    string _name;
    string _barkSound;
};
