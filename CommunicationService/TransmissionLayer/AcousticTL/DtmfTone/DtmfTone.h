#pragma once

enum class Fade{
    noFade,
    fadeIn,
    fadeOut
};

class DtmfTone {
public:
    DtmfTone();
    DtmfTone(int f1, int f2, double p1, double p2, Fade fade, int samplerate, int samplesPerTone);
    float next();
    bool isDone();

    double getP1() const;
    double getP2() const;
private:
    int f1;
    int f2;
    double p1;
    double p2;
    Fade fade;
    int samplerate;
    int samplesPerTone = 0;
    int samplesLeft = samplesPerTone;
    double r;
};
