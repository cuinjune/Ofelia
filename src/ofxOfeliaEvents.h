#pragma once

#include "m_pd.h"
#include "ofEvents.h"
#include "ofxOfeliaData.h"
#include "ofxOfelia.h"
#include <vector>
#include <string>
#include <utility>

#if defined(TARGET_EXTERNAL)
#define PD_SYS_LOCK()
#define PD_SYS_UNLOCK()
#elif defined(TARGET_STANDALONE)
#define PD_SYS_LOCK() sys_lock()
#define PD_SYS_UNLOCK() sys_unlock()
#endif

class ofxOfeliaEvents
{
public:
    typedef std::pair<ofxOfeliaData *, t_float> DataPair;
    typedef std::vector<DataPair> DataPairVec;
    static DataPairVec *getTargetDataPairVec(t_symbol *s);
    static DataPairVec::iterator findDataPair(DataPairVec &vec, ofxOfeliaData *dataPtr);
    static void addDataPair(DataPairVec &vec, ofxOfeliaData *x, t_floatarg f);
    static void removeDataPair(DataPairVec &vec, ofxOfeliaData *x);
    static void sortDataPairVec(DataPairVec &vec);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofKeyEventArgs &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofMouseEventArgs &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofResizeEventArgs &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofMessage &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofDragInfo &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, ofTouchEventArgs &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, const int e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, const std::string &e);
    static void callEventListener(ofxOfeliaData *x, t_symbol *s, const std::pair<int, int> &e);
    static void callEventListeners(DataPairVec &vec, t_symbol *s)
    {
        sortDataPairVec(vec);
        for (const auto &i : vec)
        {
            PD_SYS_LOCK();
            callEventListener(i.first, s);
            PD_SYS_UNLOCK();
        }
    }
    template<typename T>
    static void callEventListeners(DataPairVec &vec, t_symbol *s, T &e)
    {
        sortDataPairVec(vec);
        for (const auto &i : vec)
        {
            PD_SYS_LOCK();
            callEventListener(i.first, s, e);
            PD_SYS_UNLOCK();
        }
    }
    /* used for calling registered event listeners in order */
    static DataPairVec setupVec;
    static DataPairVec updateVec;
    static DataPairVec drawVec;
    static DataPairVec exitVec;
    static DataPairVec keyPressedVec;
    static DataPairVec keyReleasedVec;
    static DataPairVec mouseMovedVec;
    static DataPairVec mouseDraggedVec;
    static DataPairVec mousePressedVec;
    static DataPairVec mouseReleasedVec;
    static DataPairVec mouseScrolledVec;
    static DataPairVec mouseEnteredVec;
    static DataPairVec mouseExitedVec;
    static DataPairVec windowResizedVec;
    static DataPairVec messageReceivedVec;
    static DataPairVec draggedVec;
    static DataPairVec touchCancelledVec;
    static DataPairVec touchDoubleTapVec;
    static DataPairVec touchDownVec;
    static DataPairVec touchMovedVec;
    static DataPairVec touchUpVec;
    static DataPairVec lostFocusVec;
    static DataPairVec gotFocusVec;
    static DataPairVec gotMemoryWarningVec;
    static DataPairVec deviceOrientationChangedVec;
    static DataPairVec launchedWithURLVec;
    static DataPairVec swipeVec;
    static DataPairVec pauseVec;
    static DataPairVec stopVec;
    static DataPairVec resumeVec;
    static DataPairVec reloadTexturesVec;
    static DataPairVec backPressedVec;
    static DataPairVec okPressedVec;
    static DataPairVec cancelPressedVec;
private:
};
