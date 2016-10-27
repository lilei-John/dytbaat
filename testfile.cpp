//
// Created by quist on 10/6/16.
//

#include <iostream>
#include "StopAndWait.h"

using namespace std;

// test function that prints out an entire vector
void printVector(vector<unsigned char> b){
    cout << "|";
    for (int i = 0; i < b.size(); ++i) {
        cout << b.at(i) << "|";
    }
    cout << endl;

}

// test function that prints out an entire stream
void printStream(stringstream &c){
    cout << "|";
    char value;
    int i = 0;
    while (c >> value){
        c.seekg(i) >> value;
        cout << value << "|";
        i++;
    }
    cout << endl;
}



//  Format: vector(DataType) nameOfVector
//  myVector.push_back(value)   --> adds an element to the END of the vector and resizes it
//  myVector.at(index)          --> return element at specified index number
//  myVector.size()             --> returns an unsigned int equal to the number of elements
//  myVector.begin()            --> reads vector from first element (index 0)
//  myVector.insert(myVector.begin + int, new value)    --> adds an element BEFORE specified index number
//  myVector.erase(myVector.begin() + int)              --> removes element AT specified index number
//  myVector.clear()            --> removes all elements in vector
//  myVector.empty()            --> returns bool value if true or false

int main() {


    // test function that generates a stream of 256 chars(console readable ASCII chars)
    stringstream myStream(ios::in|ios::out|ios::app);
    int j = 0;
    char newByte = 0b00100001;
    while (j < 256) {
        myStream << newByte;
        j++;
        if (newByte == 0b01111110){
            newByte = 0b00100000;
        }
        newByte++;
    }


 //   stringstream testStream(ios::in|ios::out|ios::app);
 //   testStream << char(0b00100001);     // if we do not define char() it will input the numeric value into the stream
 //   testStream << char(0b00100011);     //  in this case 33 and 35 instead of ! and #

    StopAndWait test(myStream); // test object initializes
    char value;
    while (myStream >> value) {
        vector<unsigned char> tempFrame = test.getNextFrame();
        printVector(tempFrame);

        if (test.frameControl()){
            cout << "CRC og FLOW approved" << endl;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));



    return 0;
}