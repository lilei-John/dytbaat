//
// Created by Haukur Kristinsson on 03/11/2016.
//

#include <iostream>
#include "Logger.h"

using namespace std;

Logger::Logger(string fileName) {
    fs.open(fileName + "-" + getDateTime() + ".txt");
    fwriter.setStream(&fs);
}

Logger::~Logger() {
    fs.close();
}

void Logger::log(std::string logString) {
    if (fs.is_open()) {
        fwriter << logString << "\n";
    }
}

std::string Logger::getDateTime() {
    time_t t = time(0);
    struct tm * now = localtime( & t );
    string dateTime =   to_string(now->tm_mon + 1) +
                        to_string(now->tm_mday) +
                        to_string(now->tm_hour) +
                        to_string(now->tm_min) +
                        to_string(now->tm_sec);
    return dateTime;
}

void Logger::close() {
    if (fs.is_open()) {
        fs.close();
    }
}
