//
// Created by quist on 11/8/16.
//

#include "SelectiveRepeat.h"
using namespace std;

SelectiveRepeat::SelectiveRepeat() {

}

SelectiveRepeat::SelectiveRepeat(std::stringstream &inStream) {
    stream = &inStream;
}

void SelectiveRepeat::getData() {
    frame.clear();
    unsigned int i = index;
    while (index <  i +frameSize and !isStreamEmpty()){      // * pointer that takes the content that is pointed at
        unsigned char byte;
        stream->seekg(index) >> noskipws >> byte;                        // seekg grabber and removes the object from the stream. and -> insted of a . because it is a pointer
        frame.push_back(byte);
        index++;
    }
}

bool SelectiveRepeat::isStreamEmpty() {
    unsigned char index0;
    return !(bool)(*stream >> index0);
}

void SelectiveRepeat::addHeader() {
    if(expectingACK || isStreamEmpty()){
        frame.insert(frame.begin(), (seqNo|(1<<7)));
    }else{
        frame.insert(frame.begin(), seqNo);
    }
}

unsigned int SelectiveRepeat::calcCRC() {
#define poly 0x11021
    unsigned int remainder = 0;
    for (unsigned int num = 0; num < frame.size(); num++) {
        remainder ^= (frame[num] << 8);                 //XOR into crc top-byte
        for (int i = 0; i < 8; i++) {
            if (remainder & (1<<15)) {                  //Bit 15 is set
                remainder = (remainder << 1) ^ poly;    //Rotate and XOR with poly
            }
            else {                                      //Bit 15 is clear
                remainder = remainder << 1;             //Rotate one left
            }
        }
    }
    return remainder;
}

void SelectiveRepeat::addCRC() {
    unsigned int crcChecksum = calcCRC();               // sets the remainder to the CRC Checksum
    unsigned char crcHigh = (unsigned char)(crcChecksum >> 8);
    unsigned char crcLow = crcChecksum & 0xFF;
    frame.push_back(crcHigh);
    frame.push_back(crcLow);
}

bool SelectiveRepeat::isCrcValid() {
    return calcCRC()==0;
}

void SelectiveRepeat::frameTransmitted() {
    if (isSender) {
        cout << "Sender: " << int(frame[0]) << endl;

        for(int i = 0; i < window.size(); i++)
            cout << int(window[i][0]) << endl;
        transmit();
    }
}

void SelectiveRepeat::transmit() {
    isSender = true;

    if(framesToResend > 0){
        frame = window[window.size()-framesToResend];
        sendFrame();
        framesToResend--;
        cout << "resend" << endl;
    }else{
        if(!isStreamEmpty() && window.size() < frameBlocksize && !isWindowFull(firstOutstanding, seqNo)){
            getData();
            makeFrame();
            storeFrame();
            sendFrame();
            seqNo = (++seqNo)%totalSeqNo;
        }
        else{
            cout << seqNo << ": window full" << endl;
            expectingACK = true;
            startTimer();
        }
    }
}

void SelectiveRepeat::makeFrame() {
    addHeader();
    addCRC();
}

void SelectiveRepeat::storeFrame() {
    window.push_back(frame);
}

void SelectiveRepeat::sendFrame() {
    cout << "(" << int(frame[0]) << ")" << endl;
    cout << onFrameSendReq(frame) << endl;
    cout << "..." << endl;
//  onFrameSendTime();
}

void SelectiveRepeat::frameSplit() {
    frame.erase(frame.begin());
    for (int i = 0; i < 2; ++i) {
        frame.erase(frame.end()-1);
    }
}

void SelectiveRepeat::startTimer() {
    std::thread(&SelectiveRepeat::timer, this).detach();
}

void SelectiveRepeat::timer() {
    timerCount++;
    std::this_thread::sleep_for(std::chrono::milliseconds(timerLength));
    timeOut();
    timerCount--;
}

void SelectiveRepeat::timeOut() {
    if(expectingACK && timerCount == 1) {
        if(window[window.size()-1][0] & (1<<7)){          // If MSB is set in header, it is, second or later, timeout
            frame = window[window.size()-1];
            sendFrame();                // Send frame again
        }else{
            if(!isStreamEmpty()){       // If more data in stream
                getData();              // Send next frame from stream
                makeFrame();
                storeFrame();           // MSB in seqNo is set here
                sendFrame();
                seqNo = (++seqNo)%totalSeqNo;
            }else{                      // Send last outstanding, if stream is empty
                frame = window[window.size()-1];
                seqNo = frame[0];
                frameSplit();
                makeFrame();
                sendFrame();
            }
        }
 //     if(onTimeout) onTimeout();
    }
}

void SelectiveRepeat::receiveFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if (isCrcValid()) {
        if (isSender) {
            if (!(frame[0] &
                  (1 << 7))) {          // if MSB is set it is an ACK for stop frame! transmission complete
                incomingACK();
            } else {
                clearAll();                     // all VAR's are cleared, ready to receive or send new msg.
            }
        } else {
            if (frame.size() > 3) {
                incomingFrame();
            } else {
                frame.clear();
                frame.push_back(0b10000000);
                addCRC();
                onFrameSendReq(frame);
                clearAll();
            }
        }
    } else {
        //onCrcFail();
    }
}

void SelectiveRepeat::incomingACK() {
    expectingACK = false;
    firstOutstanding = frame[0];
    if (firstOutstanding==seqNo){
        window.clear();
        if(!isStreamEmpty()) {
            transmit();
        } else{
            frame.clear();
            makeFrame();
            storeFrame();
            sendFrame();
        }
    }else {
        for(int i = 0, j = 0; j < window.size(); ){     // Continue through all NAK's in frame
            if(frame[i]!=(window[j][0]&~(1<<7))){       // If i'th NAK != j'th frame-seqNo in window, (&~(1<<7)) to ignore piggyback saved
                window.erase(window.begin()+j);         // Delete j'th element in window (no need to resend)
            }else{                                      // Keep in window (need to resend)
                j++;
                if(i<frame.size()-3){
                    i++;
                }
            }
        }

        framesToResend = window.size();
        transmit();
    }
}

bool SelectiveRepeat::isWindowFull(int firstInWindow, int lastInWindow) {
    return (firstInWindow<=windowSize&&(((lastInWindow-firstInWindow)>= windowSize)||((lastInWindow-firstInWindow)<0))) ||  // !(0<=(lastInWindow-firstInWindow)<windowSize)
            (firstInWindow>windowSize&&(((lastInWindow-firstInWindow)>= -windowSize)||((lastInWindow-firstInWindow)<0))); // 0>(lastInWindow-firstInWindow)>=-windowSize)
}

void SelectiveRepeat::incomingFrame() {
/*    for (auto i : frame) {
        cout << i;
    }
    cout << endl;*/
        unsigned int incomingSeqNo = frame[0];
    cout << "SeqNo: " << int(incomingSeqNo) << endl;
        // Check for sender time-out (MSB is set)
        if(incomingSeqNo & (1<<7) ){    // If MSB (bit 7) is set
            incomingSeqNo &= ~(1<<7);   // Clear MSB
        //    lastInBlock = incomingSeqNo;
            isNackNeeded = true;
        }
        // NACK is needed when lastInBlock is received
        if (incomingSeqNo == lastInBlock) { // If received frame equals lastInBlock
            isNackNeeded = true;                // NACK is needed
        }

        if (!acknowledgedFrames[incomingSeqNo]) {
            // Save new frame to incomingFrames array
            acknowledgedFrames[incomingSeqNo] = true;   // Mark frame as received
            frameSplit();
            incomingFrames[incomingSeqNo] = frame;      // Copy frame to incomingFrames array
            if (incomingSeqNo == firstOutstanding) {    // If received frame equals firstOutstanding

                // Send firstOutstanding frames to stream
                while (acknowledgedFrames[firstOutstanding]) {
                    for (auto byte : incomingFrames[firstOutstanding]) {
                        *stream << noskipws << byte;    // Send to stream
                    }
                    acknowledgedFrames[(firstOutstanding + totalSeqNo - 1) %
                                       totalSeqNo] = false;             //this is weird because of the expetion: if very last frame's ACK is lost.
                    if(firstOutstanding == lastInBlock){
                        lastInBlock = (++lastInBlock) % totalSeqNo;        // Increment lastInBlock
                    }
                    firstOutstanding = (++firstOutstanding) % totalSeqNo; // Increment firstOutstanding
                }
            }
        }

    // Send NACK if needed
    if(isNackNeeded){
        isNackNeeded = false;

        // Create NAK-frame
        frame.clear();                  // Clear frame

        for (uint8_t i = firstOutstanding;
             i != (lastInBlock + 1) % totalSeqNo; i = (++i) % totalSeqNo) { // Make NACK frame
            if (!acknowledgedFrames[i]) {  // If i'th frame is corrupted
                frame.push_back(i); // Add seqNo to NACK frame
            }
        }
     /*   if (frame.size() == 0) {    // If no NACK's have been added (resulting in NACK frame still empty)
            frame.push_back(firstOutstanding);  // Add the new window's firstOutstanding to frame
        }*/

        // Adjust receive window
        lastInBlock = firstOutstanding; // Worst case: lastInBlock = firstOutstanding
         for (unsigned int i = 1, j = 0, k = lastInBlock; i < frameBlocksize && j < windowSize; i++) {    // Create new "best case" full size window

             do {
                 k = (++k) % totalSeqNo;   // Increment lastOutstanding
                 j++;
             } while (acknowledgedFrames[k]); // While acknowledged
             if (j <= windowSize) {
                 lastInBlock = k;
             }
         }

        addCRC();
        onFrameSendReq(frame);

        cout << "NAK:";
        for(int i = 0; i < frame.size()-2;i++)
            cout << " " << int(frame[i]);
        cout << endl;

        //  onFrameSendTime();
        }
}

void SelectiveRepeat::clearAll() {
    index = 0;
    seqNo = 0;
    firstOutstanding = 0;
    lastInBlock = firstOutstanding + frameBlocksize;

    frame.clear();
    window.clear();
    incomingFrames->clear();
    acknowledgedFrames[totalSeqNo] = {false};

    expectingACK = false;
    isNackNeeded = false;
    isSender = false;

    framesToResend = 0;
}

int SelectiveRepeat::getMaxFrameSize() {
    return frameSize + 3;
}


