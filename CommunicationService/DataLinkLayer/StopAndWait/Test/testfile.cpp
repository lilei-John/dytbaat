#include <iostream>
#include "../StopAndWait.h"

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
        c.seekg(i) >> std::noskipws >> value;
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

void onFrameSendCallback(vector<unsigned char> bytes){

}

int main() {
    vector<unsigned char> outData;
    vector<unsigned char> inData;
    unsigned char newByte = 0b00100001;
    string data = "juhuu hallojsa hmuuu";
    for(int i = 0; i < data.size(); i++) {
        outData.push_back((unsigned char)data[i]);
        if (data[i] == 32) {
            cout << "whitespace!" << endl;
        }
    }

    stringstream outStream(ios::in|ios::out|ios::app);
    stringstream inStream(ios::in|ios::out|ios::app);

    StopAndWait outStopAndWait(outStream);
    StopAndWait inStopAndWait(inStream);

    outStopAndWait.setOnFrameSendCallback([&](vector<unsigned char> frame) -> bool{
        //send / receive frame
        inStopAndWait.receiveFrame(frame);
        return true;
    });
    inStopAndWait.setOnFrameSendCallback([&](vector<unsigned char> frame) -> bool{
        //send / receive ack
        outStopAndWait.receiveFrame(frame);
        return true;
    });

    for (auto byte : outData){
        outStream << byte;
    }

    outStopAndWait.transmit();

    unsigned char index0;
    while(inStream >> index0){
        inData.push_back(index0);
    }
    printVector(inData);

    cout << boolalpha;
    cout << "Test succeeded: " << (inData == outData) << endl;

    return 0;
}