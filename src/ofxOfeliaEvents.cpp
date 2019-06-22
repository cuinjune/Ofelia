#include "ofxOfeliaEvents.h"

ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::setupVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::updateVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::drawVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::exitVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::keyPressedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::keyReleasedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseMovedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseDraggedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mousePressedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseReleasedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseScrolledVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseEnteredVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::mouseExitedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::windowResizedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::messageReceivedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::draggedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::touchCancelledVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::touchDoubleTapVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::touchDownVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::touchMovedVec;
ofxOfeliaEvents::DataPairVec ofxOfeliaEvents::touchUpVec;

ofxOfeliaEvents::DataPairVec *ofxOfeliaEvents::getTargetDataPairVec(t_symbol *s)
{
    if (!strcmp(s->s_name, "setup"))
        return &setupVec;
    else if (!strcmp(s->s_name, "update"))
        return &updateVec;
    else if (!strcmp(s->s_name, "draw"))
        return &drawVec;
    else if (!strcmp(s->s_name, "exit"))
        return &exitVec;
    else if (!strcmp(s->s_name, "keyPressed"))
        return &keyPressedVec;
    else if (!strcmp(s->s_name, "keyReleased"))
        return &keyReleasedVec;
    else if (!strcmp(s->s_name, "mouseMoved"))
        return &mouseMovedVec;
    else if (!strcmp(s->s_name, "mouseDragged"))
        return &mouseDraggedVec;
    else if (!strcmp(s->s_name, "mousePressed"))
        return &mousePressedVec;
    else if (!strcmp(s->s_name, "mouseReleased"))
        return &mouseReleasedVec;
    else if (!strcmp(s->s_name, "mouseScrolled"))
        return &mouseScrolledVec;
    else if (!strcmp(s->s_name, "mouseEntered"))
        return &mouseEnteredVec;
    else if (!strcmp(s->s_name, "mouseExited"))
        return &mouseExitedVec;
    else if (!strcmp(s->s_name, "windowResized"))
        return &windowResizedVec;
    else if (!strcmp(s->s_name, "messageReceived"))
        return &messageReceivedVec;
    else if (!strcmp(s->s_name, "dragged"))
        return &draggedVec;
    else if (!strcmp(s->s_name, "touchCancelled"))
        return &touchCancelledVec;
    else if (!strcmp(s->s_name, "touchDoubleTap"))
        return &touchDoubleTapVec;
    else if (!strcmp(s->s_name, "touchDown"))
        return &touchDownVec;
    else if (!strcmp(s->s_name, "touchMoved"))
        return &touchMovedVec;
    else if (!strcmp(s->s_name, "touchUp"))
        return &touchUpVec;
    else
        error("ofelia: unknown listener method '%s'", s->s_name);
    return nullptr;
}

ofxOfeliaEvents::DataPairVec::iterator ofxOfeliaEvents::findDataPair(DataPairVec &vec, ofxOfeliaData *dataPtr)
{
    return find_if(vec.begin(), vec.end(),
                   [&](const DataPair &ref) {return ref.first == dataPtr;});
}

void ofxOfeliaEvents::addDataPair(DataPairVec &vec, ofxOfeliaData *x, t_floatarg f)
{
    auto it = findDataPair(vec, x);
    if (it != vec.end())
    {
        it->second = f;
        return;
    }
    vec.push_back(std::make_pair(x, f));
}

void ofxOfeliaEvents::removeDataPair(DataPairVec &vec, ofxOfeliaData *x)
{
    auto it = findDataPair(vec, x);
    if (it != vec.end())
        vec.erase(it);
}

void ofxOfeliaEvents::sortDataPairVec(DataPairVec &vec)
{
    stable_sort(vec.begin(), vec.end(),
                [](const DataPair &a, const DataPair &b) {return a.second < b.second;});
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofEventArgs &e, t_symbol *s)
{
    x->lua.doFunction(s);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofKeyEventArgs &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Integer>(e.type));
    lua_setfield(L, -2, "type");
    lua_pushinteger(L, static_cast<lua_Integer>(e.key));
    lua_setfield(L, -2, "key");
    lua_pushinteger(L, static_cast<lua_Integer>(e.keycode));
    lua_setfield(L, -2, "keycode");
    lua_pushinteger(L, static_cast<lua_Integer>(e.scancode));
    lua_setfield(L, -2, "scancode");
    lua_pushinteger(L, static_cast<lua_Integer>(e.codepoint));
    lua_setfield(L, -2, "codepoint");
    lua_pushboolean(L, static_cast<int>(e.isRepeat));
    lua_setfield(L, -2, "isRepeat");
    lua_pushinteger(L, static_cast<lua_Integer>(e.modifiers));
    lua_setfield(L, -2, "modifiers");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofMouseEventArgs &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Integer>(e.type));
    lua_setfield(L, -2, "type");
    lua_pushnumber(L, static_cast<lua_Number>(e.x));
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, static_cast<lua_Number>(e.y));
    lua_setfield(L, -2, "y");
    lua_pushinteger(L, static_cast<lua_Integer>(e.button));
    lua_setfield(L, -2, "button");
    lua_pushnumber(L, static_cast<lua_Number>(e.scrollX));
    lua_setfield(L, -2, "scrollX");
    lua_pushnumber(L, static_cast<lua_Number>(e.scrollY));
    lua_setfield(L, -2, "scrollY");
    lua_pushinteger(L, static_cast<lua_Integer>(e.modifiers));
    lua_setfield(L, -2, "modifiers");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofResizeEventArgs &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Integer>(e.width));
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, static_cast<lua_Integer>(e.height));
    lua_setfield(L, -2, "height");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofMessage &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L);
    lua_pushstring(L, e.message.c_str());
    lua_setfield(L, -2, "message");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofDragInfo &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L); /* outermost table */
    lua_newtable(L); /* subtable "files" */
    for (size_t i = 0; i < e.files.size(); ++i)
    {
        lua_pushinteger(L, i + 1);
        lua_pushstring(L, e.files[i].c_str());
        lua_settable(L, -3);
    }
    lua_setfield(L, -2, "files");
    lua_newtable(L); /* subtable "position" */
    lua_pushnumber(L, e.position.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, e.position.y);
    lua_setfield(L, -2, "y");
    lua_setfield(L, -2, "position");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaEvents::callEventListener(ofxOfeliaData *x, ofTouchEventArgs &e, t_symbol *s)
{
    int top; if (!x->lua.isFunction(s, top)) return;
    lua_State *L = x->lua.L;
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Integer>(e.type));
    lua_setfield(L, -2, "type");
    lua_pushnumber(L, static_cast<lua_Number>(e.x));
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, static_cast<lua_Number>(e.y));
    lua_setfield(L, -2, "y");
    lua_pushinteger(L, static_cast<lua_Integer>(e.id));
    lua_setfield(L, -2, "id");
    lua_pushinteger(L, static_cast<lua_Integer>(e.time));
    lua_setfield(L, -2, "time");
    lua_pushinteger(L, static_cast<lua_Integer>(e.numTouches));
    lua_setfield(L, -2, "numTouches");
    lua_pushnumber(L, static_cast<lua_Number>(e.width));
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, static_cast<lua_Number>(e.height));
    lua_setfield(L, -2, "height");
    lua_pushnumber(L, static_cast<lua_Number>(e.angle));
    lua_setfield(L, -2, "angle");
    lua_pushnumber(L, static_cast<lua_Number>(e.minoraxis));
    lua_setfield(L, -2, "minoraxis");
    lua_pushnumber(L, static_cast<lua_Number>(e.majoraxis));
    lua_setfield(L, -2, "majoraxis");
    lua_pushnumber(L, static_cast<lua_Number>(e.pressure));
    lua_setfield(L, -2, "pressure");
    lua_pushnumber(L, static_cast<lua_Number>(e.xspeed));
    lua_setfield(L, -2, "xspeed");
    lua_pushnumber(L, static_cast<lua_Number>(e.yspeed));
    lua_setfield(L, -2, "yspeed");
    lua_pushnumber(L, static_cast<lua_Number>(e.xaccel));
    lua_setfield(L, -2, "xaccel");
    lua_pushnumber(L, static_cast<lua_Number>(e.yaccel));
    lua_setfield(L, -2, "yaccel");
    x->lua.callFunction(top);
    lua_pop(L, 1);
}
