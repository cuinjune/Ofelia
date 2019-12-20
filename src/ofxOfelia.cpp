#if defined(TARGET_STANDALONE)

#include "ofxOfelia.h"
#include "ofxOfeliaSetup.h"
#include "ofxOfeliaEvents.h"

//--------------------------------------------------------------
ofxOfelia::ofxOfelia()
{
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
}


//--------------------------------------------------------------
void ofxOfelia::setup()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::setupVec, gensym("setup"));
}

//--------------------------------------------------------------
void ofxOfelia::update()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::updateVec, gensym("update"));
}

//--------------------------------------------------------------
void ofxOfelia::draw()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::drawVec, gensym("draw"));
}

//--------------------------------------------------------------
void ofxOfelia::exit()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::exitVec, gensym("exit"));
}

//--------------------------------------------------------------
void ofxOfelia::keyPressed(ofKeyEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyPressedVec, gensym("keyPressed"), e);
}

//--------------------------------------------------------------
void ofxOfelia::keyReleased(ofKeyEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyReleasedVec, gensym("keyReleased"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseMoved(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseMovedVec, gensym("mouseMoved"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseDragged(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseDraggedVec, gensym("mouseDragged"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mousePressed(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mousePressedVec, gensym("mousePressed"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseReleased(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseReleasedVec, gensym("mouseReleased"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseScrolled(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseScrolledVec, gensym("mouseScrolled"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseEntered(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseEnteredVec, gensym("mouseEntered"), e);
}

//--------------------------------------------------------------
void ofxOfelia::mouseExited(ofMouseEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseExitedVec, gensym("mouseExited"), e);
}

//--------------------------------------------------------------
void ofxOfelia::windowResized(ofResizeEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::windowResizedVec, gensym("windowResized"), e);
}

//--------------------------------------------------------------
void ofxOfelia::messageReceived(ofMessage &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::messageReceivedVec, gensym("messageReceived"), e);
}

//--------------------------------------------------------------
void ofxOfelia::dragged(ofDragInfo &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::draggedVec, gensym("dragged"), e);
}

//--------------------------------------------------------------
void ofxOfelia::touchCancelled(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchCancelledVec, gensym("touchCancelled"), e);
}

//--------------------------------------------------------------
void ofxOfelia::touchDoubleTap(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDoubleTapVec, gensym("touchDoubleTap"), e);
}

//--------------------------------------------------------------
void ofxOfelia::touchDown(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDownVec, gensym("touchDown"), e);
}

//--------------------------------------------------------------
void ofxOfelia::touchMoved(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchMovedVec, gensym("touchMoved"), e);
}

//--------------------------------------------------------------
void ofxOfelia::touchUp(ofTouchEventArgs &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchUpVec, gensym("touchUp"), e);
}

//--------------------------------------------------------------
void ofxOfelia::lostFocus()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::lostFocusVec, gensym("lostFocus"));
}

//--------------------------------------------------------------
void ofxOfelia::gotFocus()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::gotFocusVec, gensym("gotFocus"));
}

//--------------------------------------------------------------
void ofxOfelia::gotMemoryWarning()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::gotMemoryWarningVec, gensym("gotMemoryWarning"));
}

//--------------------------------------------------------------
void ofxOfelia::deviceOrientationChanged(const int e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::deviceOrientationChangedVec, gensym("deviceOrientationChanged"), e);
}

//--------------------------------------------------------------
void ofxOfelia::launchedWithURL(const std::string &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::launchedWithURLVec, gensym("launchedWithURL"), e);
}

//--------------------------------------------------------------
void ofxOfelia::swipe(const std::pair<int, int> &e)
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::swipeVec, gensym("swipe"), e);
}

//--------------------------------------------------------------
void ofxOfelia::pause()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::pauseVec, gensym("pause"));
}

//--------------------------------------------------------------
void ofxOfelia::stop()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::stopVec, gensym("stop"));
}

//--------------------------------------------------------------
void ofxOfelia::resume()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::resumeVec, gensym("resume"));
}

//--------------------------------------------------------------
void ofxOfelia::reloadTextures()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::reloadTexturesVec, gensym("reloadTextures"));
}

//--------------------------------------------------------------
bool ofxOfelia::backPressed()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::backPressedVec, gensym("backPressed"));
    return true;
}

//--------------------------------------------------------------
void ofxOfelia::okPressed()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::okPressedVec, gensym("okPressed"));
}

//--------------------------------------------------------------
void ofxOfelia::cancelPressed()
{
    ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::cancelPressedVec, gensym("cancelPressed"));
}

//--------------------------------------------------------------
void ofxOfelia::audioIn(ofSoundBuffer &buffer)
{
    pd.audioIn(&buffer[0], buffer.getNumFrames(), buffer.getNumChannels());
}

//--------------------------------------------------------------
void ofxOfelia::audioOut(ofSoundBuffer &buffer)
{
    pd.audioOut(&buffer[0], buffer.getNumFrames(), buffer.getNumChannels());
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
