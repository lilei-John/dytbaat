//
// Created by Haukur Kristinsson on 03/11/2016.
//

#ifndef DYTBAAT_LOGGER_H
#define DYTBAAT_LOGGER_H

#include <string>
#include <fstream>
#include "FsWriter.h"
#include <ctime>
#include <chrono>


class Logger {
public:
    Logger(std::string fileName);
    ~Logger();

    void log(std::string logString);
    std::string getDateTime();
    long getTimeNow();
    void startTimer();
    void close();
private:
    std::ofstream fs;
    FStreamWriter fwriter;
    long initMs;
};


#endif //DYTBAAT_LOGGER_H
