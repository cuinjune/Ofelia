#include "ofxOfeliaSetup.h"
#include "ofxOfeliaLua.h"
#include "ofxOfeliaDefine.h"
#include "ofxOfeliaFunction.h"
#include "ofxOfeliaEmbFunction.h"
#include "ofxOfeliaEmbClass.h"
#include "ofxOfeliaGet.h"
#include "ofxOfeliaSet.h"
#include "ofxOfeliaExists.h"
#include "ofxOfeliaCreator.h"
#include "ofxOfeliaTextBuf.h"
#include "ofxOfeliaLog.h"

void ofelia_setup()
{
    if (!ofxOfeliaLua::init()) return;
    ofxOfeliaDefine::setup();
    ofxOfeliaFunction::setup();
    ofxOfeliaEmbFunction::setup();
    ofxOfeliaEmbClass::setup();
    ofxOfeliaGet::setup();
    ofxOfeliaSet::setup();
    ofxOfeliaExists::setup();
    ofxOfeliaCreator::setup();
    ofxOfeliaTextBuf::loadScript();
    ofxOfeliaLog::setLoggerChannel();
    
    /* print version info if loaded successfully */
    post("-------------------------------------------------------------------");
    post("ofelia v%d.%d : compiled on %s",
         OFELIA_MAJOR_VERSION, OFELIA_MINOR_VERSION, __DATE__);
    post("(c) 2018 Zack Lee <cuinjune@gmail.com>");
    post("-------------------------------------------------------------------");
}
