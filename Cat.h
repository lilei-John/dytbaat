//
// Created by Haukur Kristinsson on 13/09/16.
//

#include <string>
#include <iostream>

using namespace std;

#ifndef DYTBAAT_CAT_H
#define DYTBAAT_CAT_H


class Cat {

public:
    Cat();
    Cat(string name, string newMeowSound);
    string meow();

private:
    string catName;
    string meowSound;
};


#endif //DYTBAAT_CAT_H
