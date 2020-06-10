#include "ofxOfeliaSetup.h"
#include "ofxOfeliaLua.h"
#include "ofxOfeliaDefine.h"
#include "ofxOfeliaFunction.h"
#include "ofxOfeliaGet.h"
#include "ofxOfeliaSet.h"
#include "ofxOfeliaExists.h"
#include "ofxOfeliaCreator.h"
#include "ofxOfeliaTextBuf.h"
#include "ofxOfeliaLog.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

int ofeliaVersionMajor = OFELIA_MAJOR_VERSION;
int ofeliaVersionMinor = OFELIA_MINOR_VERSION;
int ofeliaVersionBugFix = OFELIA_BUGFIX_VERSION;

void ofelia_setup()
{
    /* check for pd version compatibility */
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    if (major == PD_MAJOR_VERSION_REQUIRED && minor < PD_MINOR_VERSION_REQUIRED)
    {
        error("ofelia requires Pd-%d.%d-0 or higher", PD_MAJOR_VERSION_REQUIRED, PD_MINOR_VERSION_REQUIRED);
        return;
    }
    if (!ofxOfeliaLua::init()) return;
    ofxOfeliaDefine::setup();
    ofxOfeliaFunction::setup();
    ofxOfeliaGet::setup();
    ofxOfeliaSet::setup();
    ofxOfeliaExists::setup();
    ofxOfeliaCreator::setup();
    ofxOfeliaTextBuf::loadScript();
    ofxOfeliaLog::setLoggerChannel();
    const std::string &fileName = "ofelia/CHANGES.txt";
    std::string dirResult, fileNameResult;
    if (ofxOfeliaTextBuf::canvasOpen(canvas_getcurrent(), fileName, dirResult, fileNameResult))
    {
        const std::string &fullPath = dirResult + "/" + fileNameResult;
        std::ifstream input(fullPath);
        std::string firstLine;
        std::getline(input, firstLine);
        std::sscanf(firstLine.c_str(), "v%d.%d.%d", &ofeliaVersionMajor, &ofeliaVersionMinor, &ofeliaVersionBugFix);
        input.close();
    }
    /* print version info if loaded successfully */
    post("-------------------------------------------------------------------");
    post("ofelia v%d.%d.%d : compiled on %s",
         ofeliaVersionMajor, ofeliaVersionMinor, ofeliaVersionBugFix, __DATE__);
    post("(c) 2018 Zack Lee <cuinjune@gmail.com>");
    post("-------------------------------------------------------------------");
}
