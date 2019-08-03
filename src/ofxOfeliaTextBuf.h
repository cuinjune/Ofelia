#pragma once

#include "m_pd.h"

class ofxOfeliaData;
class ofxOfeliaTextBuf
{
public:
    ofxOfeliaTextBuf(ofxOfeliaData *dataPtr)
    :dataPtr(dataPtr)
    ,previousTime(-1){};
    static void loadScript();
    void senditup();
    void openMethod();
    void closeMethod();
    void addLineMethod(t_symbol *s, int argc, t_atom *argv);
    void readMethod(t_symbol *s, int argc, t_atom *argv);
    void writeMethod(t_symbol *s, int argc, t_atom *argv);
    void free();
private:
    ofxOfeliaData *dataPtr; /* pointer to data */
    double previousTime;
};
