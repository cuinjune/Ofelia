#if TARGET_OS_IPHONE

//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//  originally from NodeBeat
//
#pragma once

#include "ofBaseSoundStream.h"
#import "SoundInputStream.h"
#import "SoundOutputStream.h"

// custom iOSSoundSTream that supports AudioBus
class ABiOSSoundStream : public ofBaseSoundStream {
    
public:
    ABiOSSoundStream();
    ~ABiOSSoundStream();
    
    /// these are not implemented on iOS
    void printDeviceList() const;
    void setDeviceID(int deviceID);
    
    void setInput(ofBaseSoundInput * soundInput);
    void setOutput(ofBaseSoundOutput * soundOutput);
    
    
    std::vector<ofSoundDevice> getDeviceList() const;
    /// currently, the number of buffers is always 1 on iOS and setting nBuffers has no effect
    /// the max buffersize is 4096
    bool setup( int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers);
    bool setup(ofBaseApp * app, int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers);
    
    void start();
    void stop();
    void close();
    
    // not implemented on iOS, always returns 0
    long unsigned long getTickCount() const;
    
    int getNumInputChannels() const;
    int getNumOutputChannels() const;
    int getSampleRate() const;
    int getBufferSize() const;
    int getDeviceID() const;
    
    static bool setMixWithOtherApps(bool bMix);
    
    SoundInputStream * getSoundInputStream();
    SoundOutputStream * getSoundOutStream();
    
private:
    
    ofBaseSoundInput * soundInputPtr;
    ofBaseSoundOutput * soundOutputPtr;
    
    SoundInputStream * soundInputStream;
    SoundOutputStream * soundOutputStream;
    
    int numOfInChannels;
    int numOfOutChannels;
    int sampleRate;
    int bufferSize;
    int numOfBuffers;
};

#endif
