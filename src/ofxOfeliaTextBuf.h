#pragma once

#include "m_pd.h"
#include <string>

class ofxOfeliaData;
class ofxOfeliaTextBuf
{
public:
    ofxOfeliaTextBuf(ofxOfeliaData *dataPtr)
    :dataPtr(dataPtr)
    ,previousTime(-1){};
    static void loadScript();
    static bool canvasOpen(const t_canvas *canvas, const std::string &fileName,
                           std::string &dirResult, std::string &fileNameResult);
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
