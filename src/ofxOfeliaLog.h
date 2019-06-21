#pragma once

#include "ofLog.h"

/* used to pass on OF logs to the pd console */
class ofxOfeliaLog : public ofBaseLoggerChannel
{
public:
    virtual ~ofxOfeliaLog(){};
    static void setLoggerChannel();
    void log(ofLogLevel level, const std::string &module, const std::string &message);
    void log(ofLogLevel level, const std::string &module, const char* format, ...) OF_PRINTF_ATTR(4, 5);
    void log(ofLogLevel level, const std::string &module, const char* format, va_list args);
private:
    static std::shared_ptr<ofxOfeliaLog> loggerChannel;
};
