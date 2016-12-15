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
    while (index <  i +FRAME_SIZE and !isStreamEmpty()) {   // * pointer that takes the content that is pointed at
        unsigned char byte;
        stream->seekg(index) >> noskipws >> byte;           // seekg grabs and removes the object from the stream. and -> insted of a . because it is a pointer
        frame.push_back(byte);
        index++;
    }
}

bool SelectiveRepeat::isStreamEmpty() {
    unsigned char index0;
    bool empty = !(bool)(*stream >> index0);
    stream->clear();
    return empty;
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
        remainder ^= (frame[num] << 8);                     //XOR into crc top-byte
        for (int i = 0; i < 8; i++) {
            if (remainder & (1<<15)) {                      //Bit 15 is set
                remainder = (remainder << 1) ^ poly;        //Rotate and XOR with poly
            }
            else {                                          //Bit 15 is clear
                remainder = remainder << 1;                 //Rotate one left
            }
        }
    }
    return remainder;
}

void SelectiveRepeat::addCRC() {
    unsigned int crcChecksum = calcCRC();                   // sets the remainder to the CRC Checksum
    uint8_t crcHigh = (uint8_t)(crcChecksum >> 8);          // splits an int into two bytes
    uint8_t crcLow = crcChecksum & 0xFF;                    // splits an int into two bytes
    frame.push_back(crcHigh);
    frame.push_back(crcLow);
}

bool SelectiveRepeat::isCrcValid() {
    return calcCRC()==0;
}

void SelectiveRepeat::frameTransmitted() {
    if (isSender) {
        transmit();
    }
}

void SelectiveRepeat::transmit() {
    isSender = true;

    if(framesToResend > 0){
        frame = window[window.size()-framesToResend];
        if(isStreamEmpty() && framesToResend==1){
            uint8_t tempSeqNo = seqNo;
            seqNo = (uint8_t)(frame[0] | (1<<7));
            frameSplit();
            makeFrame();
            seqNo = tempSeqNo;
        }
        sendFrame();
        framesToResend--;
    }else{
        if(!isStreamEmpty() && window.size() < FRAMEBLOCK_SIZE && !isWindowFull(firstOutstanding, seqNo)){
            getData();
            makeFrame();
            storeFrame();
            sendFrame();
            seqNo = (++seqNo)%TOTAL_SEQ_NO;
        }
        else{
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
    if (onFrameSend) onFrameSend(frame);
    onFrameSendReq(frame);
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
    chrono::milliseconds timerLenght = reqMaxTransmissionDuration(FRAMEBLOCK_SIZE, 4);
    timerLenght += chrono::milliseconds(200);
    timerCount++;
    std::this_thread::sleep_for(timerLenght);
    timeOut();
    timerCount--;
}

void SelectiveRepeat::timeOut() {
    if(expectingACK && timerCount == 1) {
        if(onTimeout) onTimeout();
        cout << window.size() << endl;
        if(window[window.size()-1][0] & (1<<7)){            // If MSB is set in header, it is the 2. (or later) timeout in a row
            frame = window[window.size()-1];                //
            sendFrame();                                    // resend last frame
        }else{
            if(!isStreamEmpty()){                           // If more data in stream
                getData();                                  // Send next frame from stream
                makeFrame();
                storeFrame();                               // MSB in seqNo is set here
                sendFrame();
                seqNo = (++seqNo)%TOTAL_SEQ_NO;
            }else{                                          // Send last outstanding, if stream is empty
                frame = window[window.size()-1];
                seqNo = frame[0];
                frameSplit();
                makeFrame();
                sendFrame();
            }
        }
    }
}

void SelectiveRepeat::receiveFrame(std::vector<unsigned char> aFrame) {
    frame = aFrame;
    if (isCrcValid()) {
        if (isSender) {
            if (!(frame[0] & (1 << 7))) {                   // if MSB is set it is an ACK for stop frame! transmission complete
                if(onAckReceiveTime) onAckReceiveTime(frame);
                incomingACK();
            } else {
                clearAll();                                 // all VAR's are cleared, ready to receive or send new msg.
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
                //###############################################################################################
            }
        }
    } else {
        if(onCrcFail) onCrcFail(frame);
    }
}

void SelectiveRepeat::incomingACK() {
    expectingACK = false;
    firstOutstanding = frame[0];
    if (firstOutstanding==seqNo){
        window.clear();
    }
    else {
        for(int i = 0, j = 0; j < window.size(); ){         // Continue through all NAK's in frame
            if(frame[i]!=(window[j][0]&~(1<<7))){           // If i'th NAK != j'th frame-seqNo in window, (&~(1<<7)) to ignore piggyback saved
                window.erase(window.begin()+j);             // Delete j'th element in window (no need to resend)
            }else{                                          // Keep in window (need to resend)
                j++;
                if(i<frame.size()-3){
                    i++;
                }
            }
        }

        framesToResend = window.size();
    }
    if(isStreamEmpty() && window.size()==0) {
        frame.clear();
        makeFrame();
        storeFrame();
        sendFrame();
    } else {
        transmit();
    }
}

bool SelectiveRepeat::isWindowFull(int firstInWindow, int lastInWindow) {
    return (firstInWindow<=WINDOW_SIZE&&(((lastInWindow-firstInWindow)>= WINDOW_SIZE)||((lastInWindow-firstInWindow)<0))) ||  // !(0<=(lastInWindow-firstInWindow)<WINDOW_SIZE)
            (firstInWindow>WINDOW_SIZE&&(((lastInWindow-firstInWindow)>= -WINDOW_SIZE)||((lastInWindow-firstInWindow)<0))); // 0>(lastInWindow-firstInWindow)>=-WINDOW_SIZE)
}

void SelectiveRepeat::incomingFrame() {
        unsigned int incomingSeqNo = frame[0];
        // Check for sender time-out (MSB is set)
        if(incomingSeqNo & (1<<7) ){                        // If MSB (bit 7) is set
            incomingSeqNo &= ~(1<<7);                       // Clear MSB
            isNackNeeded = true;
        }
        // NAK is needed when lastInBlock is received
        if (incomingSeqNo == lastInBlock) {                 // If received frame equals lastInBlock
            isNackNeeded = true;                            // NACK is needed
        }

        if (!acknowledgedFrames[incomingSeqNo]) {
            if (onFrameReceive) onFrameReceive(incomingSeqNo);
            // Save new frame to incomingFrames array
            acknowledgedFrames[incomingSeqNo] = true;       // Mark frame as received
            frameSplit();
            incomingFrames[incomingSeqNo] = frame;          // Copy frame to incomingFrames array
            if (incomingSeqNo == firstOutstanding) {        // If received frame equals firstOutstanding

                // Send firstOutstanding frames to stream
                while (acknowledgedFrames[firstOutstanding]) {
                    for (auto byte : incomingFrames[firstOutstanding]) {
                        *stream << noskipws << byte;                          // Send to stream
                    }
                    acknowledgedFrames[(firstOutstanding + TOTAL_SEQ_NO - 1) % TOTAL_SEQ_NO] = false; //this is weird because of the expetion: if very last frame's ACK is lost.
                    if(firstOutstanding == lastInBlock){
                        lastInBlock = (++lastInBlock) % TOTAL_SEQ_NO;         // Increment lastInBlock
                    }
                    firstOutstanding = (++firstOutstanding) % TOTAL_SEQ_NO;   // Increment firstOutstanding
                }
                if (onReceive) onReceive();
            }
        }

    // Send NACK if needed
    if(isNackNeeded){
        isNackNeeded = false;
        makeNak();
        adjustWindow();
        addCRC();
        onFrameSendReq(frame);
        //if (onFrameSend) onFrameSend();
        }
}

void SelectiveRepeat::makeNak() {
// Create NAK-frame
    frame.clear();                                          // Clear frame

    for (uint8_t i = firstOutstanding; (i != (lastInBlock + 1) % TOTAL_SEQ_NO); i = (++i) % TOTAL_SEQ_NO) {
        if (!acknowledgedFrames[i]) {                       // If i'th frame is corrupted
            frame.push_back(i);                             // Add seqNo to NACK frame
        }
    }
}

void SelectiveRepeat::adjustWindow() {
// Adjust receive window
    lastInBlock = firstOutstanding;                         // Worst case: lastInBlock = firstOutstanding
    // Create new "best case" full size window
    for (unsigned int i = 1, j = 0, k = lastInBlock; i < FRAMEBLOCK_SIZE && j < WINDOW_SIZE; i++) {
        do {
            k = (++k) % TOTAL_SEQ_NO;                       // Increment lastOutstanding
            j++;
        } while (acknowledgedFrames[k]);                    // While acknowledged
        if (j <= WINDOW_SIZE) {
            lastInBlock = k;
        }
    }
}


void SelectiveRepeat::clearAll() {
    index = 0;
    seqNo = 0;
    firstOutstanding = 0;
    lastInBlock = firstOutstanding + FRAMEBLOCK_SIZE;

    frame.clear();
    window.clear();
    incomingFrames->clear();
    for (int i = 0; i < TOTAL_SEQ_NO; ++i) {
        acknowledgedFrames[i] = false;
    }
    expectingACK = false;
    isNackNeeded = false;
    isSender = false;

    framesToResend = 0;
}

int SelectiveRepeat::getMaxFrameSize() {
    return FRAME_SIZE + 3;
}

void SelectiveRepeat::setOnTimeout(std::function<void(void)> callback) {
    onTimeout = callback;
}

void SelectiveRepeat::setOnCrcFail(std::function<void(vector<unsigned char>)> callback) {
    onCrcFail = callback;
}

void SelectiveRepeat::setOnFrameSendTime(std::function<void(vector<unsigned char>)> callback) {
    onFrameSend = callback;
}

void SelectiveRepeat::setOnAckReceiveTime(std::function<void(std::vector<unsigned char>)> callback) {
    onAckReceiveTime = callback;
}

void SelectiveRepeat::setOnFrameReceive(std::function<void(int)> callback) {
    onFrameReceive = callback;
}

int SelectiveRepeat::getTotalSeqNo() {
    return TOTAL_SEQ_NO;
}

int SelectiveRepeat::getFrameBlockSize() {
    return FRAMEBLOCK_SIZE;
}
