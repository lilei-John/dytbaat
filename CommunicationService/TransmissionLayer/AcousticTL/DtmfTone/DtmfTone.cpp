#include <math.h>
#include "DtmfTone.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

DtmfTone::DtmfTone(){}

DtmfTone::DtmfTone(int f1, int f2, double p1, double p2, Fade fade, int samplerate, int samplesPerTone)
    : f1(f1), f2(f2), p1(p1), p2(p2), fade(fade), samplerate(samplerate), samplesPerTone(samplesPerTone), r(2 * (double)M_PI / samplerate)
{}

float DtmfTone::next() {
    if (--samplesLeft < 0) throw("No samples left");
    p1 += r * f1;
    p2 += r * f2;
    double a = 1;
    if (fade == Fade::fadeIn){
        a = (double)(samplesPerTone - samplesLeft) / samplesPerTone;
    }else if(fade == Fade::fadeOut){
        a = (double)samplesLeft / samplesPerTone;
    }
    return (float)((sin(p1) + sin(p2)) * a * .5);
}

bool DtmfTone::isDone() {
    return samplesLeft == 0;
}

double DtmfTone::getP1() const {
    return p1;
}

double DtmfTone::getP2() const {
    return p2;
}
