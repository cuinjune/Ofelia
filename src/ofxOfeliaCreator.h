#pragma once

#include "m_pd.h"

class ofxOfeliaCreator
{
public:
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void setup();
private:
};
