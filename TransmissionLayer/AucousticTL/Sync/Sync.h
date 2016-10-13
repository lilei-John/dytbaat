#pragma once

class Sync {
public:
    Sync(float fpf);
    void receiveNipple(unsigned char);
    bool startSequenceReceived();
    void reset();
private:
    const float searchFramesPerToneFrame;
    const int allowedMissedSearchFrames = 1;

    int searchFramesConfirmed = 0;
    int missedHammingSearchFrames = 0;
    int startSequenceNipplesConfirmed = 0;

    unsigned char nextExpectedStartSequenceNipple();
};
