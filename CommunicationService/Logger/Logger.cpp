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
    if(isHeaderSection) {
        if (fs.is_open()) {
            fwriter << logString << "\n";
        }
    } else {
        long ms = getTimeNow() - initMs;
        if (fs.is_open()) {
            fwriter << ms << ";" << logString << "\n";
        }
    }
}

void Logger::log(float logString) {
    if(isHeaderSection) {
        if (fs.is_open()) {
            fwriter << logString << "\n";
        }
    } else {
        long ms = getTimeNow() - initMs;
        if (fs.is_open()) {
            fwriter << ms << ";" << logString << "\n";
        }
    }
}

void Logger::log(int logString) {
    if(isHeaderSection) {
        if (fs.is_open()) {
            fwriter << logString << "\n";
        }
    } else {
        long ms = getTimeNow() - initMs;
        if (fs.is_open()) {
            fwriter << ms << ";" << logString << "\n";
        }
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

long Logger::getTimeNow() {
    return std::chrono::duration_cast< chrono::milliseconds >(
            chrono::system_clock::now().time_since_epoch()
    ).count();
}

void Logger::startTimer() {
    initMs = getTimeNow();
    isHeaderSection = false;
}
