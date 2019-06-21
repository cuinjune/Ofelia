#if defined(TARGET_STANDALONE)

#include "ofxOfelia.h"
#include "ofxOfeliaSetup.h"
#include "ofxOfeliaEvents.h"

//--------------------------------------------------------------
ofxOfelia::ofxOfelia()
{
    isInited = false;
}

//--------------------------------------------------------------
ofxOfelia::~ofxOfelia()
{
    clear();
}

//--------------------------------------------------------------
bool ofxOfelia::init(const int numInChannels, // number of audio input channels to use
                     const int numOutChannels, // number of audio output channels to use
                     const int sampleRate, // audio sample rate
                     const int ticksPerBuffer, // used to compute the audio buffer len: tpb * blocksize (always 64)
                     const bool bOpenMidiInPort, // whether to open midi input port in init()
                     const bool bOpenMidiOutPort, // whether to open midi output port in init()
                     const int midiInPortNum, // midi input port number to open
                     const int midiOutPortNum, // midi output port number to open
                     const bool bOpenPatch, // whether to open a patch in init()
                     const string &patchName) // path of the patch to open (relative to "project/bin/data")
{
    // setup OF sound stream
    ofSoundStreamSettings settings;
    settings.numInputChannels = numInChannels;
    settings.numOutputChannels = numOutChannels;
    settings.sampleRate = sampleRate;
    settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
    settings.setInListener(this);
    settings.setOutListener(this);
    ofSoundStreamSetup(settings);
    
    // setup Pd
    //
    // set 4th arg to true for queued message passing using an internal ringbuffer,
    // this is useful if you need to control where and when the message callbacks
    // happen (ie. within a GUI thread)
    //
    // note: you won't see any message prints until update() is called since
    // the queued messages are processed there, this is normal
    //
    if (!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer, false))
        return false;
    
    // load externals
    ofelia_setup();
    
    // add message receiver, required if you want to recieve messages
    pd.addReceiver(*this); // automatically receives from all subscribed sources
    
    // add midi receiver, required if you want to recieve midi messages
    pd.addMidiReceiver(*this); // automatically receives from all channels
    
    // setup midi
    const int numMidiInPorts = bOpenMidiInPort ? midiIn.getNumInPorts() : 0;
    const int numMidiOutPorts = bOpenMidiOutPort ? midiOut.getNumOutPorts() : 0;
    if (numMidiInPorts)
    {
        // open midi input port by number
        if (!midiIn.openPort(midiInPortNum))
            return false;
        
        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        midiIn.ignoreTypes(false, false, false);
        
        // add this as a listener
        midiIn.addListener(this);
    }
    if (numMidiOutPorts)
    {
        // open midi output port by number
        if (!midiOut.openPort(midiOutPortNum))
            return false;
    }
    // audio processing on
    pd.start();
    
    // open patch
    if (bOpenPatch)
    {
        patch = pd.openPatch(patchName);
        if (!patch.isValid())
            return false;
    }
    isInited = true;
    return true;
}

//--------------------------------------------------------------
void ofxOfelia::clear()
{
    // audio processing off
    pd.stop();
    
    // close patch
    pd.closePatch(patch);
    
    // cleanup pd
    pd.clear();
    
    // cleanup audio
    ofSoundStreamStop();
    
    // cleanup midi
    if (midiIn.isOpen())
    {
        midiIn.closePort();
        midiIn.removeListener(this);
    }
    if (midiOut.isOpen())
        midiOut.closePort();
    
    // need to be reinited
    isInited = false;
}


//--------------------------------------------------------------
void ofxOfelia::setup(ofEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::setupVec, e, gensym("setup"));
}

//--------------------------------------------------------------
void ofxOfelia::update(ofEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::updateVec, e, gensym("update"));
}

//--------------------------------------------------------------
void ofxOfelia::draw(ofEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::drawVec, e, gensym("draw"));
}

//--------------------------------------------------------------
void ofxOfelia::exit(ofEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::exitVec, e, gensym("exit"));
}

//--------------------------------------------------------------
void ofxOfelia::keyPressed(ofKeyEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyPressedVec, e, gensym("keyPressed"));
}

//--------------------------------------------------------------
void ofxOfelia::keyReleased(ofKeyEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyReleasedVec, e, gensym("keyReleased"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseMoved(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseMovedVec, e, gensym("mouseMoved"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseDragged(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseDraggedVec, e, gensym("mouseDragged"));
}

//--------------------------------------------------------------
void ofxOfelia::mousePressed(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mousePressedVec, e, gensym("mousePressed"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseReleased(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseReleasedVec, e, gensym("mouseReleased"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseScrolled(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseScrolledVec, e, gensym("mouseScrolled"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseEntered(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseEnteredVec, e, gensym("mouseEntered"));
}

//--------------------------------------------------------------
void ofxOfelia::mouseExited(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseExitedVec, e, gensym("mouseExited"));
}

//--------------------------------------------------------------
void ofxOfelia::windowResized(ofResizeEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::windowResizedVec, e, gensym("windowResized"));
}

//--------------------------------------------------------------
void ofxOfelia::messageReceived(ofMessage &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::messageReceivedVec, e, gensym("messageReceived"));
}

//--------------------------------------------------------------
void ofxOfelia::dragged(ofDragInfo &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::draggedVec, e, gensym("dragged"));
}

//--------------------------------------------------------------
void ofxOfelia::touchCancelled(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchCancelledVec, e, gensym("touchCancelled"));
}

//--------------------------------------------------------------
void ofxOfelia::touchDoubleTap(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDoubleTapVec, e, gensym("touchDoubleTap"));
}

//--------------------------------------------------------------
void ofxOfelia::touchDown(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDownVec, e, gensym("touchDown"));
}

//--------------------------------------------------------------
void ofxOfelia::touchMoved(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchMovedVec, e, gensym("touchMoved"));
}

//--------------------------------------------------------------
void ofxOfelia::touchUp(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchUpVec, e, gensym("touchUp"));
}

//--------------------------------------------------------------
void ofxOfelia::audioReceived(float *input, int bufferSize, int nChannels)
{
    pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofxOfelia::audioRequested(float *output, int bufferSize, int nChannels)
{
    pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofxOfelia::print(const std::string& message)
{
    std::cout << message << '\n';
}

//--------------------------------------------------------------
void ofxOfelia::receiveBang(const std::string& dest)
{
}

void ofxOfelia::receiveFloat(const std::string& dest, float value)
{
}

void ofxOfelia::receiveSymbol(const std::string& dest, const std::string& symbol)
{
}

void ofxOfelia::receiveList(const std::string& dest, const List& list)
{
}

void ofxOfelia::receiveMessage(const std::string& dest, const std::string& msg, const List& list)
{
}

//--------------------------------------------------------------
void ofxOfelia::receiveNoteOn(const int channel, const int pitch, const int velocity)
{
    if (midiOut.isOpen())
        midiOut.sendNoteOn(channel, pitch, velocity);
}

void ofxOfelia::receiveControlChange(const int channel, const int controller, const int value)
{
    if (midiOut.isOpen())
        midiOut.sendControlChange(channel, controller, value);
}

// note: pgm nums are 1-128 to match pd
void ofxOfelia::receiveProgramChange(const int channel, const int value)
{
    if (midiOut.isOpen())
        midiOut.sendProgramChange(channel, value);
}

void ofxOfelia::receivePitchBend(const int channel, const int value)
{
    if (midiOut.isOpen())
        midiOut.sendPitchBend(channel, value);
}

void ofxOfelia::receiveAftertouch(const int channel, const int value)
{
    if (midiOut.isOpen())
        midiOut.sendAftertouch(channel, value);
}

void ofxOfelia::receivePolyAftertouch(const int channel, const int pitch, const int value)
{
    if (midiOut.isOpen())
        midiOut.sendPolyAftertouch(channel, pitch, value);
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void ofxOfelia::receiveMidiByte(const int port, const int byte)
{
    if (midiOut.isOpen())
        midiOut.sendMidiByte(byte);
}

//--------------------------------------------------------------
void ofxOfelia::newMidiMessage(ofxMidiMessage& msg)
{
    midiMessage = msg;
    midiChan = midiMessage.channel;
    
    if (midiMessage.getStatusString(midiMessage.status) == "Note Off")
        pd.sendNoteOn(midiChan, midiMessage.pitch, 0);
    else if (midiMessage.getStatusString(midiMessage.status) == "Note On")
        pd.sendNoteOn(midiChan, midiMessage.pitch, midiMessage.velocity);
    else if (midiMessage.getStatusString(midiMessage.status) == "Control Change")
        pd.sendControlChange(midiChan, midiMessage.control, midiMessage.value);
    else if (midiMessage.getStatusString(midiMessage.status) == "Program Change")
        pd.sendProgramChange(midiChan, midiMessage.value); // note: pgm num range is 1 - 128
    else if (midiMessage.getStatusString(midiMessage.status) == "Pitch Bend")
        pd.sendPitchBend(midiChan, midiMessage.value - 8192); //note: ofxPd uses -8192 - 8192 while
        // [bendin] returns 0 - 16383,
        // so sending a val of 2000 gives 10192 in pd
    else if (midiMessage.getStatusString(midiMessage.status) == "Aftertouch")
        pd.sendAftertouch(midiChan, midiMessage.value);
    else if (midiMessage.getStatusString(midiMessage.status) == "Poly Aftertouch")
        pd.sendPolyAftertouch(midiChan, midiMessage.pitch, midiMessage.value);
    else if (midiMessage.getStatusString(midiMessage.status) == "Sysex")
    {
        pd.sendSysex(midiMessage.portNum, midiMessage.value);       // note: pd adds +2 to the port number from
        pd.sendSysRealTime(midiMessage.portNum, midiMessage.value); // [midiin], [sysexin], & [realtimein].
        pd.sendMidiByte(midiMessage.portNum, midiMessage.value);    // so sending to port 0 gives port 2 in pd
    }
}

#endif /* TARGET_STANDALONE */
