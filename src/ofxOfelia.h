#pragma once

#if !defined(TARGET_EXTERNAL) && !defined(TARGET_STANDALONE)
#error No build target defined (TARGET_EXTERNAL or TARGET_STANDALONE)
#endif

#if defined(TARGET_STANDALONE)

#include "ofMain.h"
#include "ofxPd.h"
#include "ofxMidi.h"

// a namespace for the Pd types
using namespace pd;

class ofxOfelia : public ofBaseSoundInput, public ofBaseSoundOutput,
                  public PdReceiver, public PdMidiReceiver, public ofxMidiListener
{
public:
    ofxOfelia();
    virtual ~ofxOfelia();
    
    bool init(const int numInChannels = 0, const int numOutChannels = 2,
              const int sampleRate = 44100, const int ticksPerBuffer = 8,
              const bool bOpenMidiInPort = false, const bool bOpenMidiOutPort = false,
              const int midiInPortNum = 0, const int midiOutPortNum = 0,
              const bool bOpenPatch = true, const string &patchName = "pd/main.pd");
    void clear();
    
    // event listeners
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    void mouseMoved(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mousePressed(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void mouseScrolled(ofMouseEventArgs &e);
    void mouseEntered(ofMouseEventArgs &e);
    void mouseExited(ofMouseEventArgs &e);
    void windowResized(ofResizeEventArgs &e);
    void messageReceived(ofMessage &e);
    void dragged(ofDragInfo &e);
    void touchCancelled(ofTouchEventArgs &e);
    void touchDoubleTap(ofTouchEventArgs &e);
    void touchDown(ofTouchEventArgs &e);
    void touchMoved(ofTouchEventArgs &e);
    void touchUp(ofTouchEventArgs &e);
    
    // ios specific listeners
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(const int e);
    void launchedWithURL(const std::string &e);
    
    // android specific listeners
    void swipe(const std::pair<int, int> &e);
    void pause();
    void stop();
    void resume();
    void reloadTextures();
    bool backPressed();
    void okPressed();
    void cancelPressed();
    
    // audio callbacks
    void audioIn(ofSoundBuffer &buffer);
    void audioOut(ofSoundBuffer &buffer);
    
    // pd message receiver callbacks
    void print(const std::string& message);
    void receiveBang(const std::string& dest);
    void receiveFloat(const std::string& dest, float value);
    void receiveSymbol(const std::string& dest, const std::string& symbol);
    void receiveList(const std::string& dest, const List& list);
    void receiveMessage(const std::string& dest, const std::string& msg, const List& list);
    
    // pd midi receiver callbacks
    void receiveNoteOn(const int channel, const int pitch, const int velocity);
    void receiveControlChange(const int channel, const int controller, const int value);
    void receiveProgramChange(const int channel, const int value);
    void receivePitchBend(const int channel, const int value);
    void receiveAftertouch(const int channel, const int value);
    void receivePolyAftertouch(const int channel, const int pitch, const int value);
    void receiveMidiByte(const int port, const int byte);
    
    // ofxMidi
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    // ofxPd
    ofxPd pd;
    Patch patch;
    
    // ofxMidi
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;
    
private:
    ofxMidiMessage midiMessage;
    int midiChan = 0;
};

#endif /* TARGET_STANDALONE */
