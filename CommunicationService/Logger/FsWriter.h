//
// Created by Haukur Kristinsson on 03/11/2016.
//

#ifndef DYTBAAT_FSWRITER_H
#define DYTBAAT_FSWRITER_H

#include <fstream>

class FStreamWriter
{
public:
    FStreamWriter() {};
    FStreamWriter(std::ofstream *f) : f(f) {}

    void setStream(std::ofstream *fs) {f = fs;}

    template <typename T>
    FStreamWriter &operator<<(const T &x)
    {
        std::lock_guard<std::mutex> lock(mtx); // <- Mutex to make it safe-thread
        (*f) << x;
        return *this;
    }

private:
    std::ofstream *f;
    std::mutex mtx;
    // ...
};

#endif //DYTBAAT_FSWRITER_H
