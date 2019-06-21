#pragma once

#include "m_pd.h"
#include "ofEvents.h"
#include "ofxOfeliaData.h"
#include "ofxOfelia.h"

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
    static void callEventListener(ofxOfeliaData *x, ofEventArgs &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofKeyEventArgs &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofMouseEventArgs &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofResizeEventArgs &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofMessage &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofDragInfo &e, t_symbol *s);
    static void callEventListener(ofxOfeliaData *x, ofTouchEventArgs &e, t_symbol *s);
    template<typename T>
    static void callEventListeners(DataPairVec &vec, T &e, t_symbol *s)
    {
        sortDataPairVec(vec);
        for (const auto &i : vec)
        {
            PD_SYS_LOCK();
            callEventListener(i.first, e, s);
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
private:
};
