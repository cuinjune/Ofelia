#if TARGET_OS_IPHONE

//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//
//  originally from NodeBeat
//
#include "ABiOSSoundStream.h"
#include "ofxiOSSoundStreamDelegate.h"
#include "ofBaseApp.h"
#import <AVFoundation/AVFoundation.h>

//------------------------------------------------------------------------------
ABiOSSoundStream::ABiOSSoundStream() {
    soundInputStream = NULL;
    soundOutputStream = NULL;
    
    soundInputPtr = NULL;
    soundOutputPtr = NULL;
    
    numOfInChannels = 0;
    numOfOutChannels = 0;
    sampleRate = 0;
    bufferSize = 0;
    numOfBuffers = 0;
}

//------------------------------------------------------------------------------
ABiOSSoundStream::~ABiOSSoundStream() {
    close();
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::printDeviceList() const{
    //
}

std::vector<ofSoundDevice> ABiOSSoundStream::getDeviceList() const{
    
    return vector<ofSoundDevice>();
    
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::setDeviceID(int _deviceID) {
    //
}


int ABiOSSoundStream::getDeviceID()  const{
    return 0;
}
//------------------------------------------------------------------------------
void ABiOSSoundStream::setInput(ofBaseSoundInput * soundInput) {
    soundInputPtr = soundInput;
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
    soundOutputPtr = soundOutput;
}

//------------------------------------------------------------------------------
bool ABiOSSoundStream::setup(int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers) {
    close();
    
    this->numOfOutChannels = numOfOutChannels;
    this->numOfInChannels = numOfInChannels;
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
    this->numOfBuffers = numOfBuffers;
    
    if(numOfInChannels > 0) {
        soundInputStream = [[SoundInputStream alloc] initWithNumOfChannels:numOfInChannels
                                                            withSampleRate:sampleRate
                                                            withBufferSize:bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundInputApp:soundInputPtr];
        ((SoundInputStream *)soundInputStream).delegate = delegate;
        [(SoundInputStream *)soundInputStream start];
    }
    
    if(numOfOutChannels > 0) {
        soundOutputStream = [[SoundOutputStream alloc] initWithNumOfChannels:numOfOutChannels
                                                              withSampleRate:sampleRate
                                                              withBufferSize:bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundOutputApp:soundOutputPtr];
        ((SoundInputStream *)soundOutputStream).delegate = delegate;
        [(SoundInputStream *)soundOutputStream start];
    }
    
    bool bOk = (soundInputStream != NULL) || (soundOutputStream != NULL);
    return bOk;
}

//------------------------------------------------------------------------------
bool ABiOSSoundStream::setup(ofBaseApp * app, int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers){
    setInput(app);
    setOutput(app);
    bool bOk = setup(numOfOutChannels, numOfInChannels, sampleRate, bufferSize, numOfBuffers);
    return bOk;
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::start(){
    if(soundInputStream != NULL) {
        [(SoundInputStream *)soundInputStream start];
    }
    
    if(soundOutputStream != NULL) {
        [(SoundOutputStream *)soundOutputStream start];
    }
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::stop(){
    if(soundInputStream != NULL) {
        [(SoundInputStream *)soundInputStream stop];
    }
    
    if(soundOutputStream != NULL) {
        [(SoundOutputStream *)soundOutputStream stop];
    }
}

//------------------------------------------------------------------------------
void ABiOSSoundStream::close(){
    if(soundInputStream != NULL) {
        [soundInputStream.delegate release];
        [soundInputStream setDelegate:nil];
        [soundInputStream stop];
        [soundInputStream release];
        soundInputStream = NULL;
    }
    
    if(soundOutputStream != NULL) {
        [soundInputStream.delegate release];
        [soundInputStream setDelegate:nil];
        [soundOutputStream stop];
        [soundOutputStream release];
        soundOutputStream = NULL;
    }
    
    numOfInChannels = 0;
    numOfOutChannels = 0;
    sampleRate = 0;
    bufferSize = 0;
    numOfBuffers = 0;
}

//------------------------------------------------------------------------------
long unsigned long ABiOSSoundStream::getTickCount() const{
    return 0;
}

//------------------------------------------------------------------------------
int ABiOSSoundStream::getNumOutputChannels() const{
    return numOfOutChannels;
}

//------------------------------------------------------------------------------
int ABiOSSoundStream::getNumInputChannels() const{
    return numOfInChannels;
}

//------------------------------------------------------------------------------
int ABiOSSoundStream::getSampleRate() const{
    return sampleRate;
}

//------------------------------------------------------------------------------
int ABiOSSoundStream::getBufferSize() const{
    return bufferSize;
}


//------------------------------------------------------------------------------
SoundInputStream * ABiOSSoundStream::getSoundInputStream(){
    return soundInputStream;
}

//------------------------------------------------------------------------------
SoundOutputStream * ABiOSSoundStream::getSoundOutStream(){
    return soundOutputStream;
}

//------------------------------------------------------------------------------
bool ABiOSSoundStream::setMixWithOtherApps(bool bMix){
    AVAudioSession * audioSession = [AVAudioSession sharedInstance];
    bool success = false;
    
#ifdef __IPHONE_6_0
    if(bMix) {
        if([audioSession respondsToSelector:@selector(setCategory:withOptions:error:)]) {
            if([audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
                             withOptions:AVAudioSessionCategoryOptionMixWithOthers
                                   error:nil]) {
                success = true;
            }
        }
    } else {
#endif
        
        // this is the default category + options setup
        // Note: using a sound input stream will set the category to PlayAndRecord
        if([audioSession setCategory:AVAudioSessionCategorySoloAmbient error:nil]) {
            success = true;
        }
        
#ifdef __IPHONE_6_0
    }
#endif
    
    if(!success) {
        ofLogError("ABiOSSoundStream") << "setMixWithOtherApps(): couldn't set app audio session category";
    }
    
    return success;
}

#endif

