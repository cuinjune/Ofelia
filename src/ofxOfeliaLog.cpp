#include "ofxOfeliaLog.h"
#include "ofUtils.h"
#include "m_pd.h"

std::shared_ptr<ofxOfeliaLog> ofxOfeliaLog::loggerChannel;

void ofxOfeliaLog::setLoggerChannel()
{
    loggerChannel = std::make_shared<ofxOfeliaLog>();
    ofSetLoggerChannel(loggerChannel);
}

void ofxOfeliaLog::log(ofLogLevel level, const std::string &module, const std::string &message)
{
    logpost(NULL, 4 - level, "%s: %s", module.c_str(), message.c_str());
}

void ofxOfeliaLog::log(ofLogLevel level, const std::string &module, const char* format, ...)
{
    char buf[MAXPDSTRING];
    std::va_list args;
    va_start(args, format);
    std::vsnprintf(buf, MAXPDSTRING - 1, format, args);
    va_end(args);
    logpost(NULL, 4 - level, "%s: %s", module.c_str(), buf);
}

void ofxOfeliaLog::log(ofLogLevel level, const std::string &module, const char* format, std::va_list args)
{
    char buf[MAXPDSTRING];
    std::snprintf(buf, MAXPDSTRING, "%s", ofVAArgsToString(format, args).c_str());
    logpost(NULL, 4 - level, "%s: %s", module.c_str(), buf);
}
