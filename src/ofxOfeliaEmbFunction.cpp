#include "ofxOfeliaEmbFunction.h"
#include "ofxOfeliaDefine.h"
#include <new>
#include <cstdio>
#include <vector>
#include <sstream>

#define _FORMAT_OFEVENT_SCRIPT1(listenerName, allEvents) \
t_float order = 50.0f; \
t_float enabled = 1.0f; \
if (argc == 1) \
order = atom_getfloat(argv + 0); \
else if (argc == 2) \
{ \
    order = atom_getfloat(argv + 0); \
    enabled = atom_getfloat(argv + 1); \
} \
else if (argc != 0) \
{ \
    error("%s: requires 1 or 2 creation arguments", s->s_name); \
    return 0; \
} \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(2); \
SETSYMBOL(args.data() + 0, gensym(";")); \
std::ostringstream ss; \
ss << \
"local order, enabled = " << order << ", " << enabled << "\n" \
"function M.new()\n" \
"  ofWindow.addListener(\"" << listenerName << "\", this, order)\n" \
"end\n" \
"function M.free()\n" \
"  ofWindow.removeListener(\"" << listenerName << "\", this)\n" \
"end\n" \
"function M.setOrder(f)\n" \
"  order = f\n" \
"  ofWindow.addListener(\"" << listenerName << "\", this, order)\n" \
"end\n" \
"function M.setEnabled(f)\n" \
"  enabled = f\n" \
"end\n" \
"function M." << listenerName << "(e)\n" \
"  if enabled == 0 then\n" \
"    return\n" \
"  end\n" \
"  return " << allEvents << "\n" \
"end"; \
SETSYMBOL(args.data() + 1, gensym(ss.str().c_str())); \
return definePtr->newMethod(s, args.size(), args.data());

#define _FORMAT_OFEVENT_SCRIPT2(listenerName, allEvents, simplifiedEvents) \
t_float order = 50.0f; \
t_float enabled = 1.0f; \
t_float simplify = 1.0f; \
if (argc == 1) \
order = atom_getfloat(argv + 0); \
else if (argc == 2) \
{ \
    order = atom_getfloat(argv + 0); \
    enabled = atom_getfloat(argv + 1); \
} \
else if (argc == 3) \
{ \
    order = atom_getfloat(argv + 0); \
    enabled = atom_getfloat(argv + 1); \
    simplify = atom_getfloat(argv + 2); \
} \
else if (argc != 0) \
{ \
    error("%s: requires 1, 2 or 3 creation arguments", s->s_name); \
    return 0; \
} \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(2); \
SETSYMBOL(args.data() + 0, gensym(";")); \
std::ostringstream ss; \
ss << \
"local order, enabled, simplify = " << order << ", " << enabled << ", " << simplify << "\n" \
"function M.new()\n" \
"  ofWindow.addListener(\"" << listenerName << "\", this, order)\n" \
"end\n" \
"function M.free()\n" \
"  ofWindow.removeListener(\"" << listenerName << "\", this)\n" \
"end\n" \
"function M.setOrder(f)\n" \
"  order = f\n" \
"  ofWindow.addListener(\"" << listenerName << "\", this, order)\n" \
"end\n" \
"function M.setEnabled(f)\n" \
"  enabled = f\n" \
"end\n" \
"function M.setSimplify(f)\n" \
"  simplify = f\n" \
"end\n" \
"function M." << listenerName << "(e)\n" \
"  if enabled == 0 then\n" \
"    return\n" \
"  end\n" \
"  if simplify == 0 then\n" \
"    return " << allEvents << "\n" \
"  end\n" \
"  return " << simplifiedEvents << "\n" \
"end"; \
SETSYMBOL(args.data() + 1, gensym(ss.str().c_str())); \
return definePtr->newMethod(s, args.size(), args.data());

#define FORMAT_OFEVENT_SCRIPT(listenerName) \
_FORMAT_OFEVENT_SCRIPT1(listenerName, "e")

#define FORMAT_OFKEYEVENT_SCRIPT(listenerName, simplifiedEvents) \
_FORMAT_OFEVENT_SCRIPT2(listenerName, "{e.type, e.key, e.keycode, e.scancode, e.codepoint, e.isRepeat, e.modifiers}", simplifiedEvents)

#define FORMAT_OFMOUSEEVENT_SCRIPT(listenerName, simplifiedEvents) \
_FORMAT_OFEVENT_SCRIPT2(listenerName, "{e.type, e.x, e.y, e.button, e.scrollX, e.scrollY, e.modifiers}", simplifiedEvents)

#define FORMAT_OFRESIZEEVENT_SCRIPT(listenerName) \
_FORMAT_OFEVENT_SCRIPT1(listenerName, "{e.width, e.height}")

#define FORMAT_OFMESSAGEEVENT_SCRIPT(listenerName) \
_FORMAT_OFEVENT_SCRIPT1(listenerName, "e.message")

#define FORMAT_OFTOUCHEVENT_SCRIPT(listenerName, simplifiedEvents) \
_FORMAT_OFEVENT_SCRIPT2(listenerName, "{e.type, e.x, e.y, e.id, e.time, e.numTouches, e.width, e.height, e.angle, e.minoraxis, e.majoraxis, e.pressure, e.xspeed, e.yspeed, e.xaccel, e.yaccel}", simplifiedEvents)

#define FORMAT_OFSWIPEEVENT_SCRIPT(listenerName) \
_FORMAT_OFEVENT_SCRIPT1(listenerName, "{e.dir, e.id}")

#define FORMAT_OFGETDRAWORDER_SCRIPT(objectName, returnVar) \
t_symbol *s = gensym(objectName); \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(2); \
SETSYMBOL(args.data() + 0, gensym(";")); \
std::ostringstream ss; \
ss << \
"local t = ofWindow.getListenerData(\"draw\")\n" \
"if #t == 0 then\n" \
"  return nil\n" \
"end\n" \
"return " << returnVar; \
SETSYMBOL(args.data() + 1, gensym(ss.str().c_str())); \
return definePtr->function.newMethod(s, args.size(), args.data());

#define FORMAT_OFCANVAS_GETTER_SCRIPT(objectName, methodName) \
t_symbol *s = gensym(objectName); \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(2); \
SETSYMBOL(args.data() + 0, gensym(";")); \
std::ostringstream ss; \
ss << \
"local canvas = ofCanvas(this, " << f << ")\n" \
"function M.bang()\n" \
"  local r = canvas:" << methodName << "()\n" \
"  return r\n" \
"end"; \
SETSYMBOL(args.data() + 1, gensym(ss.str().c_str())); \
return definePtr->newMethod(s, args.size(), args.data());

#define _FORMAT_OFOSC_TILDE_SCRIPT(className, objectName, sFlag, methodName, sArgs) \
t_symbol *s = gensym(objectName); \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(4); \
SETSYMBOL(args.data() + 0, gensym(sFlag)); \
SETFLOAT(args.data() + 1, f); \
SETSYMBOL(args.data() + 2, gensym(";")); \
std::ostringstream ss; \
ss << \
"local osc = " << className << "()\n" \
"local inlet = ofInlet(this)\n" \
"function M.float(f)\n" \
"  inlet:setSignalInlet(f)\n" \
"end\n" \
"function M.setPhase(f)\n" \
"  osc:setPhase(f)\n" \
"end\n" \
"function M.perform(" << sArgs << ")\n" \
"  return osc:" << methodName << "(" << sArgs << ")\n" \
"end"; \
SETSYMBOL(args.data() + 3, gensym(ss.str().c_str())); \
return definePtr->newMethod(s, args.size(), args.data());

#define FORMAT_OFOSC_TILDE_SCRIPT(objectName, sFlag, methodName, sArgs) \
_FORMAT_OFOSC_TILDE_SCRIPT("ofOsc", objectName, sFlag, methodName, sArgs)

#define FORMAT_OFBLOSC_TILDE_SCRIPT(objectName, sFlag, methodName, sArgs) \
_FORMAT_OFOSC_TILDE_SCRIPT("ofBlOsc", objectName, sFlag, methodName, sArgs)

#define FORMAT_OFFILTER_TILDE_SCRIPT(objectName, sFlag, methodName, sArgs) \
t_symbol *s = gensym(objectName); \
definePtr->data.isEmbedded = true; \
definePtr->data.embName = s; \
std::vector<t_atom> args; \
args.resize(3); \
SETSYMBOL(args.data() + 0, gensym(sFlag)); \
SETSYMBOL(args.data() + 1, gensym(";")); \
std::ostringstream ss; \
ss << \
"local filter = ofFilter()\n" \
"function M.clear()\n" \
"  filter:clear()\n" \
"end\n" \
"function M.perform(" << sArgs << ")\n" \
"  return filter:" << methodName << "(" << sArgs << ")\n" \
"end"; \
SETSYMBOL(args.data() + 2, gensym(ss.str().c_str())); \
return definePtr->newMethod(s, args.size(), args.data());

void *ofxOfeliaEmbFunction::newMethod_default(t_symbol *s, int argc, t_atom *argv)
{
    return definePtr->embBase.functionMapScript(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newMethod_ofSetup(t_symbol *s, int argc, t_atom *argv)
{
    t_float order = 50.0f;
    t_float enabled = 1.0f;
    if (argc == 1)
        order = atom_getfloat(argv + 0);
    else if (argc == 2)
    {
        order = atom_getfloat(argv + 0);
        enabled = atom_getfloat(argv + 1);
    }
    else if (argc != 0)
    {
        error("%s: requires 1 or 2 creation arguments", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local clock = ofClock(this, \"setup\")\n"
    "local order, enabled = " << order << ", " << enabled << "\n"
    "function M.new()\n"
    "  ofWindow.addListener(\"setup\", this, order)\n"
    "  if ofWindow.exists then\n"
    "    clock:delay(0)\n"
    "  end\n"
    "end\n"
    "function M.free()\n"
    "  ofWindow.removeListener(\"setup\", this)\n"
    "end\n"
    "function M.setOrder(f)\n"
    "  order = f\n"
    "  ofWindow.addListener(\"setup\", this, order)\n"
    "end\n"
    "function M.setEnabled(f)\n"
    "  enabled = f\n"
    "end\n"
    "function M.setup()\n"
    "  if enabled == 0 then\n"
    "    return\n"
    "  end\n"
    "  return nil\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofUpdate(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("update");
}

void *ofxOfeliaEmbFunction::newMethod_ofDraw(t_symbol *s, int argc, t_atom *argv)
{
    t_float order = 50.0f;
    t_float enabled = 1.0f;
    t_float styleAuto = 1.0f;
    t_float matrixAuto = 1.0f;
    if (argc == 1)
        order = atom_getfloat(argv + 0);
    else if (argc == 2)
    {
        order = atom_getfloat(argv + 0);
        enabled = atom_getfloat(argv + 1);
    }
    else if (argc == 4)
    {
        order = atom_getfloat(argv + 0);
        enabled = atom_getfloat(argv + 1);
        styleAuto = atom_getfloat(argv + 2);
        matrixAuto = atom_getfloat(argv + 3);
    }
    else if (argc != 0)
    {
        error("%s: requires 1, 2 or 4 creation arguments", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "local order, enabled, styleAuto, matrixAuto = " << order << ", " << enabled << ", " << styleAuto << ", " << matrixAuto << "\n"
    "function M.new()\n"
    "  ofWindow.addListener(\"draw\", this, order)\n"
    "end\n"
    "function M.free()\n"
    "  ofWindow.removeListener(\"draw\", this)\n"
    "end\n"
    "function M.setOrder(f)\n"
    "  order = f\n"
    "  ofWindow.addListener(\"draw\", this, order)\n"
    "end\n"
    "function M.setEnabled(f)\n"
    "  enabled = f\n"
    "end\n"
    "function M.setStyleAuto(f)\n"
    "  styleAuto = f\n"
    "end\n"
    "function M.setMatrixAuto(f)\n"
    "  matrixAuto = f\n"
    "end\n"
    "function M.setAuto(f)\n"
    "  styleAuto, matrixAuto = f, f\n"
    "end\n"
    "function M.draw()\n"
    "  if enabled == 0 then\n"
    "    return\n"
    "  end\n"
    "  if styleAuto ~= 0 then\n"
    "    ofPushStyle()\n"
    "  end\n"
    "  if matrixAuto ~= 0 then\n"
    "    ofPushMatrix()\n"
    "  end\n"
    "  outlet:outletBang(0)\n"
    "  if matrixAuto ~= 0 then\n"
    "    ofPopMatrix()\n"
    "  end\n"
    "  if styleAuto ~= 0 then\n"
    "    ofPopStyle()\n"
    "  end\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofExit(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("exit");
}

void *ofxOfeliaEmbFunction::newMethod_ofKeyPressed(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFKEYEVENT_SCRIPT("keyPressed", "e.key");
}

void *ofxOfeliaEmbFunction::newMethod_ofKeyReleased(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFKEYEVENT_SCRIPT("keyReleased", "e.key");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseMoved(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseMoved", "{e.x, e.y}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseDragged(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseDragged", "{e.x, e.y, e.button}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMousePressed(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mousePressed", "{e.x, e.y, e.button}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseReleased(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseReleased", "{e.x, e.y, e.button}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseScrolled(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseScrolled", "{e.x, e.y, e.scrollX, e.scrollY}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseEntered(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseEntered", "{e.x, e.y}");
}

void *ofxOfeliaEmbFunction::newMethod_ofMouseExited(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMOUSEEVENT_SCRIPT("mouseExited", "{e.x, e.y}");
}

void *ofxOfeliaEmbFunction::newMethod_ofWindowResized(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFRESIZEEVENT_SCRIPT("windowResized");
}

void *ofxOfeliaEmbFunction::newMethod_ofMessageReceived(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFMESSAGEEVENT_SCRIPT("messageReceived");
}

void *ofxOfeliaEmbFunction::newMethod_ofDragged(t_symbol *s, int argc, t_atom *argv)
{
    t_float order = 50.0f;
    t_float enabled = 1.0f;
    if (argc == 1)
        order = atom_getfloat(argv + 0);
    else if (argc == 2)
    {
        order = atom_getfloat(argv + 0);
        enabled = atom_getfloat(argv + 1);
    }
    else if (argc != 0)
    {
        error("%s: requires 1 or 2 creation arguments", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(3);
    SETSYMBOL(args.data() + 0, gensym("-c12"));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "local order, enabled = " << order << ", " << enabled << "\n"
    "function M.new()\n"
    "  ofWindow.addListener(\"dragged\", this, order)\n"
    "end\n"
    "function M.free()\n"
    "  ofWindow.removeListener(\"dragged\", this)\n"
    "end\n"
    "function M.setOrder(f)\n"
    "  order = f\n"
    "  ofWindow.addListener(\"dragged\", this, order)\n"
    "end\n"
    "function M.setEnabled(f)\n"
    "  enabled = f\n"
    "end\n"
    "function M.dragged(e)\n"
    "  if enabled == 0 then\n"
    "    return\n"
    "  end\n"
    "  outlet:outletList(1, {e.position.x, e.position.y})\n"
    "  outlet:outletList(0, e.files)\n"
    "end";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofTouchCancelled(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFTOUCHEVENT_SCRIPT("touchCancelled", "{e.x, e.y, e.id}");
}

void *ofxOfeliaEmbFunction::newMethod_ofTouchDoubleTap(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFTOUCHEVENT_SCRIPT("touchDoubleTap", "{e.x, e.y, e.id}");
}

void *ofxOfeliaEmbFunction::newMethod_ofTouchDown(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFTOUCHEVENT_SCRIPT("touchDown", "{e.x, e.y, e.id}");
}

void *ofxOfeliaEmbFunction::newMethod_ofTouchMoved(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFTOUCHEVENT_SCRIPT("touchMoved", "{e.x, e.y, e.id}");
}

void *ofxOfeliaEmbFunction::newMethod_ofTouchUp(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFTOUCHEVENT_SCRIPT("touchUp", "{e.x, e.y, e.id}");
}

void *ofxOfeliaEmbFunction::newMethod_ofLostFocus(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("lostFocus");
}

void *ofxOfeliaEmbFunction::newMethod_ofGotFocus(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("gotFocus");
}

void *ofxOfeliaEmbFunction::newMethod_ofGotMemoryWarning(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("gotMemoryWarning");
}

void *ofxOfeliaEmbFunction::newMethod_ofDeviceOrientationChanged(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("deviceOrientationChanged");
}

void *ofxOfeliaEmbFunction::newMethod_ofLaunchedWithURL(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("launchedWithURL");
}

void *ofxOfeliaEmbFunction::newMethod_ofSwipe(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFSWIPEEVENT_SCRIPT("swipe");
}

void *ofxOfeliaEmbFunction::newMethod_ofPause(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("pause");
}

void *ofxOfeliaEmbFunction::newMethod_ofStop(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("stop");
}

void *ofxOfeliaEmbFunction::newMethod_ofResume(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("resume");
}

void *ofxOfeliaEmbFunction::newMethod_ofReloadTextures(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("reloadTextures");
}

void *ofxOfeliaEmbFunction::newMethod_ofBackPressed(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("backPressed");
}

void *ofxOfeliaEmbFunction::newMethod_ofOkPressed(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("okPressed");
}

void *ofxOfeliaEmbFunction::newMethod_ofCancelPressed(t_symbol *s, int argc, t_atom *argv)
{
    FORMAT_OFEVENT_SCRIPT("cancelPressed");
}

void *ofxOfeliaEmbFunction::newMethod_ofGetFirstDrawOrder()
{
    FORMAT_OFGETDRAWORDER_SCRIPT("ofGetFirstDrawOrder", "t[1].order");
}

void *ofxOfeliaEmbFunction::newMethod_ofGetLastDrawOrder()
{
    FORMAT_OFGETDRAWORDER_SCRIPT("ofGetLastDrawOrder", "t[#t].order");
}

void *ofxOfeliaEmbFunction::newMethod_ofTrueTypeFontSettings(t_symbol *s, int argc, t_atom *argv)
{
    if (argc)
    {
        if (argv[0].a_type == A_SYMBOL)
            definePtr->data.sym = argv[0].a_w.w_symbol;
        else if (!(argv[0].a_type == A_FLOAT && argv[0].a_w.w_float == 0)) /* skip unset $1 argument */
        {
            error("%s: wrong argument type for module name", s->s_name);
            return 0;
        }
    }
    definePtr->data.initSym();
    t_symbol *moduleName = definePtr->data.sym;
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(10);
    SETSYMBOL(args.data() + 0, moduleName);
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local log = ofLog(\"ofTrueTypeFontSettings\")\n"
    "M.trueTypeFontSettings = ofTrueTypeFontSettings(\"\", 0)\n"
    "function M.bang()\n"
    "  return M.trueTypeFontSettings\n"
    "end\n"
    "function M.free()\n"
    "  M.trueTypeFontSettings = nil\n"
    "end\n"
    "function M.set(av)\n"
    "  M.trueTypeFontSettings = ofTrueTypeFontSettings(av[1], av[2])\n"
    "end\n"
    "function M.antialiased(b)\n"
    "  M.trueTypeFontSettings.antialiased = b ~= 0\n"
    "end\n"
    "function M.contours(b)\n"
    "  M.trueTypeFontSettings.contours = b ~= 0\n"
    "end\n"
    "function M.simplifyAmt(f)\n"
    "  M.trueTypeFontSettings.simplifyAmt = f\n"
    "end\n"
    "function M.dpi(i)\n"
    "  M.trueTypeFontSettings.dpi = i\n"
    "end\n"
    "function M.direction(i)\n"
    "  M.trueTypeFontSettings.direction = i\n"
    "end\n";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "function M.addRanges(s)\n"
    "  local alphabet = s\n"
    "  if alphabet == \"Emoji\" then\n"
    "    alphabet = ofAlphabet_Emoji\n"
    "  elseif alphabet == \"Japanese\" then\n"
    "    alphabet = ofAlphabet_Japanese\n"
    "  elseif alphabet == \"Chinese\" then\n"
    "    alphabet = ofAlphabet_Chinese\n"
    "  elseif alphabet == \"Korean\" then\n"
    "    alphabet = ofAlphabet_Korean\n"
    "  elseif alphabet == \"Arabic\" then\n"
    "    alphabet = ofAlphabet_Arabic\n"
    "  elseif alphabet == \"Devanagari\" then\n"
    "    alphabet = ofAlphabet_Devanagari\n"
    "  elseif alphabet == \"Latin\" then\n"
    "    alphabet = ofAlphabet_Latin\n"
    "  elseif alphabet == \"Greek\" then\n"
    "    alphabet = ofAlphabet_Greek\n"
    "  elseif alphabet == \"Cyrillic\" then\n"
    "    alphabet = ofAlphabet_Cyrillic\n"
    "  else\n"
    "    log:error(\"unknown alphabet : \" .. alphabet)\n"
    "    return\n"
    "  end\n"
    "  M.trueTypeFontSettings:addRanges(alphabet)\n"
    "end\n";
    SETSYMBOL(args.data() + 3, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "function M.addRange(s)\n"
    "  local range = s\n"
    "  if range == \"Space\" then\n"
    "    range = ofUnicode.Space\n"
    "  elseif range == \"IdeographicSpace\" then\n"
    "    range = ofUnicode.IdeographicSpace\n"
    "  elseif range == \"Latin\" then\n"
    "    range = ofUnicode.Latin\n"
    "  elseif range == \"Latin1Supplement\" then\n"
    "    range = ofUnicode.Latin1Supplement\n"
    "  elseif range == \"LatinA\" then\n"
    "    range = ofUnicode.LatinA\n"
    "  elseif range == \"Greek\" then\n"
    "    range = ofUnicode.Greek\n"
    "  elseif range == \"Cyrillic\" then\n"
    "    range = ofUnicode.Cyrillic\n"
    "  elseif range == \"Arabic\" then\n"
    "    range = ofUnicode.Arabic\n"
    "  elseif range == \"ArabicSupplement\" then\n"
    "    range = ofUnicode.ArabicSupplement\n"
    "  elseif range == \"ArabicExtendedA\" then\n"
    "    range = ofUnicode.ArabicExtendedA\n"
    "  elseif range == \"Devanagari\" then\n"
    "    range = ofUnicode.Devanagari\n";
    SETSYMBOL(args.data() + 4, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "  elseif range == \"HangulJamo\" then\n"
    "    range = ofUnicode.HangulJamo\n"
    "  elseif range == \"VedicExtensions\" then\n"
    "    range = ofUnicode.VedicExtensions\n"
    "  elseif range == \"LatinExtendedAdditional\" then\n"
    "    range = ofUnicode.LatinExtendedAdditional\n"
    "  elseif range == \"GreekExtended\" then\n"
    "    range = ofUnicode.GreekExtended\n"
    "  elseif range == \"GeneralPunctuation\" then\n"
    "    range = ofUnicode.GeneralPunctuation\n"
    "  elseif range == \"SuperAndSubScripts\" then\n"
    "    range = ofUnicode.SuperAndSubScripts\n"
    "  elseif range == \"CurrencySymbols\" then\n"
    "    range = ofUnicode.CurrencySymbols\n"
    "  elseif range == \"LetterLikeSymbols\" then\n"
    "    range = ofUnicode.LetterLikeSymbols\n"
    "  elseif range == \"NumberForms\" then\n"
    "    range = ofUnicode.NumberForms\n"
    "  elseif range == \"Arrows\" then\n"
    "    range = ofUnicode.Arrows\n";
    SETSYMBOL(args.data() + 5, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "  elseif range == \"MathOperators\" then\n"
    "    range = ofUnicode.MathOperators\n"
    "  elseif range == \"MiscTechnical\" then\n"
    "    range = ofUnicode.MiscTechnical\n"
    "  elseif range == \"BoxDrawing\" then\n"
    "    range = ofUnicode.BoxDrawing\n"
    "  elseif range == \"BlockElement\" then\n"
    "    range = ofUnicode.BlockElement\n"
    "  elseif range == \"GeometricShapes\" then\n"
    "    range = ofUnicode.GeometricShapes\n"
    "  elseif range == \"MiscSymbols\" then\n"
    "    range = ofUnicode.MiscSymbols\n"
    "  elseif range == \"Dingbats\" then\n"
    "    range = ofUnicode.Dingbats\n"
    "  elseif range == \"Hiragana\" then\n"
    "    range = ofUnicode.Hiragana\n"
    "  elseif range == \"Katakana\" then\n"
    "    range = ofUnicode.Katakana\n"
    "  elseif range == \"HangulCompatJamo\" then\n"
    "    range = ofUnicode.HangulCompatJamo\n"
    "  elseif range == \"KatakanaPhoneticExtensions\" then\n"
    "    range = ofUnicode.KatakanaPhoneticExtensions\n";
    SETSYMBOL(args.data() + 6, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "  elseif range == \"CJKLettersAndMonths\" then\n"
    "    range = ofUnicode.CJKLettersAndMonths\n"
    "  elseif range == \"CJKUnified\" then\n"
    "    range = ofUnicode.CJKUnified\n"
    "  elseif range == \"DevanagariExtended\" then\n"
    "    range = ofUnicode.DevanagariExtended\n"
    "  elseif range == \"HangulExtendedA\" then\n"
    "    range = ofUnicode.HangulExtendedA\n"
    "  elseif range == \"HangulSyllables\" then\n"
    "    range = ofUnicode.HangulSyllables\n"
    "  elseif range == \"HangulExtendedB\" then\n"
    "    range = ofUnicode.HangulExtendedB\n"
    "  elseif range == \"AlphabeticPresentationForms\" then\n"
    "    range = ofUnicode.AlphabeticPresentationForms\n"
    "  elseif range == \"ArabicPresFormsA\" then\n"
    "    range = ofUnicode.ArabicPresFormsA\n"
    "  elseif range == \"ArabicPresFormsB\" then\n"
    "    range = ofUnicode.ArabicPresFormsB\n";
    SETSYMBOL(args.data() + 7, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "  elseif range == \"KatakanaHalfAndFullwidthForms\" then\n"
    "    range = ofUnicode.KatakanaHalfAndFullwidthForms\n"
    "  elseif range == \"KanaSupplement\" then\n"
    "    range = ofUnicode.KanaSupplement\n"
    "  elseif range == \"RumiNumericalSymbols\" then\n"
    "    range = ofUnicode.RumiNumericalSymbols\n"
    "  elseif range == \"ArabicMath\" then\n"
    "    range = ofUnicode.ArabicMath\n"
    "  elseif range == \"MiscSymbolsAndPictographs\" then\n"
    "    range = ofUnicode.MiscSymbolsAndPictographs\n"
    "  elseif range == \"Emoticons\" then\n"
    "    range = ofUnicode.Emoticons\n"
    "  elseif range == \"TransportAndMap\" then\n"
    "    range = ofUnicode.TransportAndMap\n";
    SETSYMBOL(args.data() + 8, gensym(ss.str().c_str()));
    ss.str("");
    ss <<
    "  elseif range == \"EnclosedCharacters\" then\n"
    "    range = ofUnicode.EnclosedCharacters\n"
    "  elseif range == \"Uncategorized\" then\n"
    "    range = ofUnicode.Uncategorized\n"
    "  elseif range == \"AdditionalEmoticons\" then\n"
    "    range = ofUnicode.AdditionalEmoticons\n"
    "  elseif range == \"AdditionalTransportAndMap\" then\n"
    "    range = ofUnicode.AdditionalTransportAndMap\n"
    "  elseif range == \"OtherAdditionalSymbols\" then\n"
    "    range = ofUnicode.OtherAdditionalSymbols\n"
    "  else\n"
    "    log:error(\"unknown range : \" .. range)\n"
    "    return\n"
    "  end\n"
    "  M.trueTypeFontSettings:addRange(range)\n"
    "end";
    SETSYMBOL(args.data() + 9, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofRequire(t_symbol *s)
{
    t_symbol *s2 = gensym("ofRequire");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s2;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local name = \"" << s->s_name << "\"\n"
    "function M.bang()\n"
    "  local m = require(name)\n"
    "  return m\n"
    "end\n"
    "function M.float(f)\n"
    "  return {f, M.bang()}\n"
    "end\n"
    "function M.symbol(s)\n"
    "  return {s, M.bang()}\n"
    "end\n"
    "function M.pointer(p)\n"
    "  return {p, M.bang()}\n"
    "end\n"
    "function M.list(av)\n"
    "  av[#av+1] = M.bang()\n"
    "  return av\n"
    "end\n"
    "function M.name(s)\n"
    "  name = s\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s2, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofExpr(t_symbol *s, int argc, t_atom *argv)
{
    std::vector<std::string> exprVec;
    exprVec.reserve(9);
    std::ostringstream expr;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                {
                    exprVec.push_back(expr.str());
                    expr.str("");
                }
            }
            else
                expr << argv->a_w.w_symbol->s_name << ' ';
        }
        else if (argv->a_type == A_FLOAT)
            expr << argv->a_w.w_float << ' ';
        argc--; argv++;
    }
    if (expr.str() != "")
        exprVec.push_back(expr.str());
    const size_t numExpr = exprVec.size();
    if (numExpr > 9)
    {
        error("%s: maximum number of expressions exceeded", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(4 + numExpr);
    char buf[5];
    std::snprintf(buf, 5, "-c1%zu", numExpr);
    SETSYMBOL(args.data() + 0, gensym(buf));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "function M.bang()\n"
    "  M.anything(nil)\n"
    "end\n"
    "function M.float(f)\n"
    "  M.anything(f)\n"
    "end\n"
    "function M.symbol(s)\n"
    "  M.anything(s)\n"
    "end\n"
    "function M.pointer(p)\n"
    "  M.anything(p)\n"
    "end\n"
    "function M.list(l)\n"
    "  M.anything(l)\n"
    "end\n"
    "function M.anything(a)";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "  M.out(" << i << ", " << exprVec[i] << ")";
        SETSYMBOL(args.data() + 2 + (numExpr - i), gensym(ss.str().c_str()));
    }
    ss.str("");
    ss <<
    "end\n"
    "function M.out(n, r)\n"
    "  local t = type(r)\n"
    "  if t == \"nil\" then\n"
    "    outlet:outletBang(n)\n"
    "  elseif t == \"boolean\" then\n"
    "    outlet:outletFloat(n, r and 1 or 0)\n"
    "  elseif t == \"number\" then\n"
    "    outlet:outletFloat(n, r)\n"
    "  elseif t == \"string\" then\n"
    "    outlet:outletSymbol(n, r)\n"
    "  elseif t == \"userdata\" then\n"
    "    outlet:outletPointer(n, r)\n"
    "  elseif t == \"table\" then\n"
    "    outlet:outletList(n, r)\n"
    "  end\n"
    "end";
    SETSYMBOL(args.data() + 3 + numExpr, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void ofxOfeliaEmbFunction::ofExpr_tilde_getNumInlets(const std::string &exprStr, size_t &numInlets)
{
    size_t pos = exprStr.find('a');
    while (pos != std::string::npos)
    {
        if (pos == exprStr.length() - 1)
            return;
        const char next = exprStr[pos + 1];
        if (next >= '1' && next <= '9')
        {
            if (pos > 0)
            {
                const char prev = exprStr[pos - 1];
                if (prev != ' ' && prev != '+' && prev != '-' && prev != '*' && prev != '/' &&
                    prev != '%' && prev != '^' && prev != '=' && prev != '>' && prev != '<')
                {
                    pos = exprStr.find('a', pos + 3);
                    continue;
                }
            }
            const size_t digit = next - '0';
            if (digit > numInlets)
                numInlets = digit;
        }
        pos = exprStr.find('a', pos + 3);
    }
}

void *ofxOfeliaEmbFunction::newMethod_ofExpr_tilde(t_symbol *s, int argc, t_atom *argv)
{
    std::vector<std::string> exprVec;
    exprVec.reserve(9);
    std::ostringstream expr;
    size_t numInlets = 1;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                {
                    ofExpr_tilde_getNumInlets(expr.str(), numInlets);
                    exprVec.push_back(expr.str());
                    expr.str("");
                }
            }
            else
                expr << argv->a_w.w_symbol->s_name << ' ';
        }
        else if (argv->a_type == A_FLOAT)
            expr << argv->a_w.w_float << ' ';
        argc--; argv++;
    }
    if (expr.str() != "")
    {
        ofExpr_tilde_getNumInlets(expr.str(), numInlets);
        exprVec.push_back(expr.str());
        expr.str("");
    }
    const size_t numExpr = exprVec.size();
    if (numExpr > 9)
    {
        error("%s: maximum number of expressions exceeded", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(4 + numExpr);
    char buf[5];
    std::snprintf(buf, 5, "-s%zu%zu", numInlets, numExpr);
    SETSYMBOL(args.data() + 0, gensym(buf));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local inlet = ofInlet(this)\n"
    "function M.float(f)\n"
    "  inlet:setSignalInlet(f)\n"
    "end\n"
    "function M.perform(x1";
    for (size_t i = 1; i < numInlets; ++i)
        ss << ", x" << i + 1;
    ss <<
    ")\n"
    "  local y1 = {}\n";
    for (size_t i = 1; i < numExpr; ++i)
        ss << "  local y" << i + 1 << " = {}\n";
    ss <<
    "  for i = 1, #x1 do\n";
    for (size_t i = 0; i < numInlets; ++i)
        ss << "    local a" << i + 1 << " = x" << i + 1 << "[i]\n";
    for (size_t i = numInlets; i < numExpr; ++i)
        ss << "    local a" << i + 1 << " = 0\n";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "    y" << i + 1 << "[i] = " << exprVec[i];
        SETSYMBOL(args.data() + 3 + i, gensym(ss.str().c_str()));
    }
    ss.str("");
    ss <<
    "  end\n"
    "  return y1";
    for (size_t i = 1; i < numExpr; ++i)
        ss << ", y" << i + 1;
    ss << "\nend";
    SETSYMBOL(args.data() + 3 + numExpr, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofIf(t_symbol *s, int argc, t_atom *argv)
{
    std::vector<std::string> exprVec;
    exprVec.reserve(9);
    std::ostringstream expr;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                {
                    exprVec.push_back(expr.str());
                    expr.str("");
                }
            }
            else
            {
                const std::string &str = argv->a_w.w_symbol->s_name;
                if (str == "then")
                    expr << "then M.out(" << exprVec.size() << ", ";
                else if (str == "else")
                    expr << ") else M.out(" << exprVec.size() << ", ";
                else if (str == "elseif")
                    expr << ") elseif ";
                else
                    expr << str << ' ';
            }
        }
        else if (argv->a_type == A_FLOAT)
            expr << argv->a_w.w_float << ' ';
        argc--; argv++;
    }
    if (expr.str() != "")
        exprVec.push_back(expr.str());
    const size_t numExpr = exprVec.size();
    if (numExpr > 9)
    {
        error("%s: maximum number of statements exceeded", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(4 + numExpr);
    char buf[5];
    std::snprintf(buf, 5, "-c1%zu", numExpr);
    SETSYMBOL(args.data() + 0, gensym(buf));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "function M.bang()\n"
    "  M.anything(nil)\n"
    "end\n"
    "function M.float(f)\n"
    "  M.anything(f)\n"
    "end\n"
    "function M.symbol(s)\n"
    "  M.anything(s)\n"
    "end\n"
    "function M.pointer(p)\n"
    "  M.anything(p)\n"
    "end\n"
    "function M.list(l)\n"
    "  M.anything(l)\n"
    "end\n"
    "function M.anything(a)";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "  if " << exprVec[i] << ") end";
        SETSYMBOL(args.data() + 2 + (numExpr - i), gensym(ss.str().c_str()));
    }
    ss.str("");
    ss <<
    "end\n"
    "function M.out(n, r)\n"
    "  local t = type(r)\n"
    "  if t == \"nil\" then\n"
    "    outlet:outletBang(n)\n"
    "  elseif t == \"boolean\" then\n"
    "    outlet:outletFloat(n, r and 1 or 0)\n"
    "  elseif t == \"number\" then\n"
    "    outlet:outletFloat(n, r)\n"
    "  elseif t == \"string\" then\n"
    "    outlet:outletSymbol(n, r)\n"
    "  elseif t == \"userdata\" then\n"
    "    outlet:outletPointer(n, r)\n"
    "  elseif t == \"table\" then\n"
    "    outlet:outletList(n, r)\n"
    "  end\n"
    "end";
    SETSYMBOL(args.data() + 3 + numExpr, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofIf_tilde(t_symbol *s, int argc, t_atom *argv)
{
    std::vector<std::string> exprVec;
    exprVec.reserve(9);
    std::ostringstream expr;
    size_t numInlets = 1;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                {
                    ofExpr_tilde_getNumInlets(expr.str(), numInlets);
                    exprVec.push_back(expr.str());
                    expr.str("");
                }
            }
            else
            {
                const std::string &str = argv->a_w.w_symbol->s_name;
                if (str == "then" || str == "else")
                    expr << str << " y" << exprVec.size() + 1 << "[i] = ";
                else
                    expr << str << ' ';
            }
        }
        else if (argv->a_type == A_FLOAT)
            expr << argv->a_w.w_float << ' ';
        argc--; argv++;
    }
    if (expr.str() != "")
    {
        ofExpr_tilde_getNumInlets(expr.str(), numInlets);
        exprVec.push_back(expr.str());
        expr.str("");
    }
    const size_t numExpr = exprVec.size();
    if (numExpr > 9)
    {
        error("%s: maximum number of statements exceeded", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(4 + numExpr);
    char buf[5];
    std::snprintf(buf, 5, "-s%zu%zu", numInlets, numExpr);
    SETSYMBOL(args.data() + 0, gensym(buf));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local inlet = ofInlet(this)\n"
    "function M.float(f)\n"
    "  inlet:setSignalInlet(f)\n"
    "end\n"
    "function M.perform(x1";
    for (size_t i = 1; i < numInlets; ++i)
        ss << ", x" << i + 1;
    ss <<
    ")\n"
    "  local y1 = {}\n";
    for (size_t i = 1; i < numExpr; ++i)
        ss << "  local y" << i + 1 << " = {}\n";
    ss <<
    "  for i = 1, #x1 do\n";
    for (size_t i = 0; i < numInlets; ++i)
        ss << "    local a" << i + 1 << " = x" << i + 1 << "[i]\n";
    for (size_t i = numInlets; i < numExpr; ++i)
        ss << "    local a" << i + 1 << " = 0\n";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "    if " << exprVec[i] << "end";
        SETSYMBOL(args.data() + 3 + i, gensym(ss.str().c_str()));
    }
    ss.str("");
    ss <<
    "  end\n"
    "  return y1";
    for (size_t i = 1; i < numExpr; ++i)
        ss << ", y" << i + 1;
    ss << "\nend";
    SETSYMBOL(args.data() + 3 + numExpr, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

bool ofxOfeliaEmbFunction::ofFor_isOperator(char ch)
{
    if (ch == '%' || (ch >= '*' && ch <= '/') || (ch >= '<' && ch <= '>') || ch == '^' || ch == '~')
        return true;
    return false;
}

void *ofxOfeliaEmbFunction::newMethod_ofFor(t_symbol *s, int argc, t_atom *argv)
{
    std::vector<std::string> exprVec;
    exprVec.reserve(9);
    std::ostringstream expr;
    bool isExpectingOperator = false;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                {
                    exprVec.push_back(expr.str());
                    expr.str("");
                    isExpectingOperator = false;
                }
            }
            else
            {
                const std::string &str = argv->a_w.w_symbol->s_name;
                if (isExpectingOperator)
                {
                    if (ofFor_isOperator(str[0]))
                        expr << str << ' ';
                    else
                        expr << ", " << str << ' ';
                }
                else
                    expr << str << ' ';
                isExpectingOperator = !ofFor_isOperator(str.back());
            }
        }
        else if (argv->a_type == A_FLOAT)
        {
            if (isExpectingOperator)
                expr << ", " << argv->a_w.w_float << ' ';
            else
                expr << argv->a_w.w_float << ' ';
            isExpectingOperator = true;
        }
        argc--; argv++;
    }
    if (expr.str() != "")
        exprVec.push_back(expr.str());
    const size_t numExpr = exprVec.size();
    if (numExpr > 9)
    {
        error("%s: maximum number of statements exceeded", s->s_name);
        return 0;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(4 + numExpr + numExpr);
    char buf[5];
    std::snprintf(buf, 5, "-c1%zu", numExpr);
    SETSYMBOL(args.data() + 0, gensym(buf));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "function M.bang()\n"
    "  M.anything(nil)\n"
    "end\n"
    "function M.float(f)\n"
    "  M.anything(f)\n"
    "end\n"
    "function M.symbol(s)\n"
    "  M.anything(s)\n"
    "end\n"
    "function M.pointer(p)\n"
    "  M.anything(p)\n"
    "end\n"
    "function M.list(l)\n"
    "  M.anything(l)\n"
    "end\n"
    "function M.anything(a)";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "  for i" << i << " = " << exprVec[i] << "do";
        SETSYMBOL(args.data() + 3 + i, gensym(ss.str().c_str()));
    }
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss.str("");
        ss <<
        "    outlet:outletFloat(" << i << ", i" << i << ")";
        SETSYMBOL(args.data() + 3 + numExpr + (numExpr - 1 - i), gensym(ss.str().c_str()));
    }
    ss.str("");
    for (size_t i = 0; i < numExpr; ++i)
    {
        ss <<
        "  end\n";
    }
    ss <<
    "end";
    SETSYMBOL(args.data() + 3 + numExpr + numExpr, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofTable(t_symbol *s, int argc, t_atom *argv)
{
    std::ostringstream expr;
    bool isExpectingOperator = false;
    while (argc)
    {
        if (argv->a_type == A_SYMBOL)
        {
            if (*argv->a_w.w_symbol->s_name == ';')
            {
                if (expr.str() != "")
                    isExpectingOperator = true;
            }
            else
            {
                const std::string &str = argv->a_w.w_symbol->s_name;
                if (isExpectingOperator)
                {
                    if (ofFor_isOperator(str[0]))
                        expr << str << ' ';
                    else
                        expr << ", " << str << ' ';
                }
                else
                    expr << str << ' ';
                isExpectingOperator = !ofFor_isOperator(str.back());
            }
        }
        else if (argv->a_type == A_FLOAT)
        {
            if (isExpectingOperator)
                expr << ", " << argv->a_w.w_float << ' ';
            else
                expr << argv->a_w.w_float << ' ';
            isExpectingOperator = true;
        }
        argc--; argv++;
    }
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(3);
    SETSYMBOL(args.data() + 0, gensym("-c11"));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "return {" << expr.str() << "}";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    return definePtr->function.newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofGetDollarZero(t_floatarg f)
{
    FORMAT_OFCANVAS_GETTER_SCRIPT("ofGetDollarZero", "getDollarZero");
}

void *ofxOfeliaEmbFunction::newMethod_ofCanvasName(t_floatarg f)
{
    FORMAT_OFCANVAS_GETTER_SCRIPT("ofCanvasName", "getName");
}

void *ofxOfeliaEmbFunction::newMethod_ofCanvasIndex(t_floatarg f)
{
    FORMAT_OFCANVAS_GETTER_SCRIPT("ofCanvasIndex", "getIndex");
}

void *ofxOfeliaEmbFunction::newMethod_ofCanvasArgs(t_floatarg f)
{
    t_symbol *s = gensym("ofCanvasArgs");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local canvas = ofCanvas(this, " << f << ")\n"
    "function M.bang()\n"
    "  return canvas:getArgs()\n"
    "end\n"
    "function M.float(f)\n"
    "  return canvas:setArgs({f})\n"
    "end\n"
    "function M.symbol(s)\n"
    "  return canvas:setArgs({s})\n"
    "end\n"
    "function M.list(av)\n"
    "  return canvas:setArgs(av)\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofCanvasPosition(t_floatarg f)
{
    t_symbol *s = gensym("ofCanvasPosition");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local canvas = ofCanvas(this, " << f << ")\n"
    "function M.bang()\n"
    "  return canvas:getPosition()\n"
    "end\n"
    "function M.list(iv)\n"
    "  return canvas:setPosition(iv[1], iv[2])\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofGetDir(t_floatarg f)
{
    FORMAT_OFCANVAS_GETTER_SCRIPT("ofGetDir", "getDir");
}

void *ofxOfeliaEmbFunction::newMethod_ofMakeFileName(t_floatarg f)
{
    t_symbol *s = gensym("ofMakeFileName");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local canvas = ofCanvas(this, " << f << ")\n"
    "function M.symbol(s)\n"
    "  return canvas:makeFileName(s)\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofRemoveCanvas(t_floatarg f)
{
    FORMAT_OFCANVAS_GETTER_SCRIPT("ofRemoveCanvas", "remove");
}

void *ofxOfeliaEmbFunction::newMethod_ofSine_tilde(t_floatarg f)
{
    FORMAT_OFOSC_TILDE_SCRIPT("ofSine~", "-s11", "sine", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofTriangle_tilde(t_floatarg f)
{
    FORMAT_OFOSC_TILDE_SCRIPT("ofTriangle~", "-s11", "triangle", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofSaw_tilde(t_floatarg f)
{
    FORMAT_OFOSC_TILDE_SCRIPT("ofSaw~", "-s11", "saw", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofSquare_tilde(t_floatarg f)
{
    FORMAT_OFOSC_TILDE_SCRIPT("ofSquare~", "-s11", "square", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofPulse_tilde(t_floatarg f)
{
    FORMAT_OFOSC_TILDE_SCRIPT("ofPulse~", "-s21", "pulse", "s1, s2");
}

void *ofxOfeliaEmbFunction::newMethod_ofBlTriangle_tilde(t_floatarg f)
{
    FORMAT_OFBLOSC_TILDE_SCRIPT("ofBlTriangle~", "-s11", "triangle", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofBlSaw_tilde(t_floatarg f)
{
    FORMAT_OFBLOSC_TILDE_SCRIPT("ofBlSaw~", "-s11", "saw", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofBlSquare_tilde(t_floatarg f)
{
    FORMAT_OFBLOSC_TILDE_SCRIPT("ofBlSquare~", "-s11", "square", "s1");
}

void *ofxOfeliaEmbFunction::newMethod_ofBlPulse_tilde(t_floatarg f)
{
    FORMAT_OFBLOSC_TILDE_SCRIPT("ofBlPulse~", "-s21", "pulse", "s1, s2");
}

void *ofxOfeliaEmbFunction::newMethod_ofLowPass_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofLowPass~", "-s31", "lowPass", "s1, s2, s3");
}

void *ofxOfeliaEmbFunction::newMethod_ofHighPass_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofHighPass~", "-s31", "highPass", "s1, s2, s3");
}

void *ofxOfeliaEmbFunction::newMethod_ofBandPass_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofBandPass~", "-s31", "bandPass", "s1, s2, s3");
}

void *ofxOfeliaEmbFunction::newMethod_ofNotch_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofNotch~", "-s31", "notch", "s1, s2, s3");
}

void *ofxOfeliaEmbFunction::newMethod_ofPeaking_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofPeaking~", "-s41", "peaking", "s1, s2, s3, s4");
}

void *ofxOfeliaEmbFunction::newMethod_ofLowShelf_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofLowShelf~", "-s41", "lowShelf", "s1, s2, s3, s4");
}

void *ofxOfeliaEmbFunction::newMethod_ofHighShelf_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofHighShelf~", "-s41", "highShelf", "s1, s2, s3, s4");
}

void *ofxOfeliaEmbFunction::newMethod_ofAllPass_tilde()
{
    FORMAT_OFFILTER_TILDE_SCRIPT("ofAllPass~", "-s31", "allPass", "s1, s2, s3");
}

void *ofxOfeliaEmbFunction::newMethod_ofReverb_tilde()
{
    t_symbol *s = gensym("ofReverb~");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(3);
    SETSYMBOL(args.data() + 0, gensym("-s22"));
    SETSYMBOL(args.data() + 1, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local reverb = ofReverb()\n"
    "function M.setRoomSize(f)\n"
    "  reverb:setRoomSize(f)\n"
    "end\n"
    "function M.setDamp(f)\n"
    "  reverb:setDamp(f)\n"
    "end\n"
    "function M.setWidth(f)\n"
    "  reverb:setWidth(f)\n"
    "end\n"
    "function M.setWet(f)\n"
    "  reverb:setWet(f)\n"
    "end\n"
    "function M.setDry(f)\n"
    "  reverb:setDry(f)\n"
    "end\n"
    "function M.setMode(f)\n"
    "  reverb:setMode(f)\n"
    "end\n"
    "function M.perform(s1, s2)\n"
    "  return reverb:process(s1, s2)\n"
    "end";
    SETSYMBOL(args.data() + 2, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofOscSender()
{
    t_symbol *s = gensym("ofOscSender");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "M.oscSender = ofOscSender()\n"
    "function M.free()\n"
    "  M.oscSender:clear()\n"
    "  M.oscSender = nil\n"
    "end\n"
    "function M.setup(av)\n"
    "  outlet:outletAnything(0, {\"setup\", M.oscSender:setup(av[1], av[2])})\n"
    "end\n"
    "function M.clear()\n"
    "  M.oscSender:clear()\n"
    "end\n"
    "function M.send(av)\n"
    "  M.oscSender:send(av)\n"
    "end\n"
    "function M.getHost()\n"
    "  outlet:outletAnything(0, {\"getHost\", M.oscSender:getHost()})\n"
    "end\n"
    "function M.getPort()\n"
    "  outlet:outletAnything(0, {\"getPort\", M.oscSender:getPort()})\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newMethod_ofOscReceiver()
{
    t_symbol *s = gensym("ofOscReceiver");
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    std::vector<t_atom> args;
    args.resize(2);
    SETSYMBOL(args.data() + 0, gensym(";"));
    std::ostringstream ss;
    ss <<
    "local outlet = ofOutlet(this)\n"
    "M.oscReceiver = ofOscReceiver(this)\n"
    "function M.free()\n"
    "  M.oscReceiver:stop()\n"
    "  M.oscReceiver = nil\n"
    "end\n"
    "function M.setup(i)\n"
    "  outlet:outletAnything(0, {\"setup\", M.oscReceiver:setup(i)})\n"
    "end\n"
    "function M.start()\n"
    "  outlet:outletAnything(0, {\"start\", M.oscReceiver:start()})\n"
    "end\n"
    "function M.stop()\n"
    "  M.oscReceiver:stop()\n"
    "end\n"
    "function M.setPollingInterval(f)\n"
    "  M.oscReceiver:setPollingInterval(f)\n"
    "end\n"
    "function M.isListening()\n"
    "  outlet:outletAnything(0, {\"isListening\", M.oscReceiver:isListening()})\n"
    "end\n"
    "function M.hasWaitingMessages()\n"
    "  outlet:outletAnything(0, {\"hasWaitingMessages\", M.oscReceiver:hasWaitingMessages()})\n"
    "end\n"
    "function M.getPort()\n"
    "  outlet:outletAnything(0, {\"getPort\", M.oscReceiver:getPort()})\n"
    "end\n"
    "function M.received(av)\n"
    "  table.insert(av, 1, \"received\")\n"
    "  outlet:outletAnything(0, av)\n"
    "end";
    SETSYMBOL(args.data() + 1, gensym(ss.str().c_str()));
    return definePtr->newMethod(s, args.size(), args.data());
}

void *ofxOfeliaEmbFunction::newWrapper_default(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_default(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofSetup(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofSetup(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofUpdate(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofUpdate(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofDraw(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofDraw(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofExit(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofExit(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofKeyPressed(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofKeyPressed(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofKeyReleased(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofKeyReleased(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseMoved(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseMoved(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseDragged(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseDragged(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMousePressed(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMousePressed(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseReleased(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseReleased(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseScrolled(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseScrolled(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseEntered(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseEntered(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMouseExited(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMouseExited(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofWindowResized(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofWindowResized(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMessageReceived(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMessageReceived(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofDragged(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofDragged(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTouchCancelled(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTouchCancelled(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTouchDoubleTap(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTouchDoubleTap(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTouchDown(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTouchDown(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTouchMoved(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTouchMoved(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTouchUp(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTouchUp(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofLostFocus(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofLostFocus(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofGotFocus(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGotFocus(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofGotMemoryWarning(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGotMemoryWarning(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofDeviceOrientationChanged(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofDeviceOrientationChanged(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofLaunchedWithURL(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofLaunchedWithURL(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofSwipe(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofSwipe(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofPause(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofPause(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofStop(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofStop(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofResume(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofResume(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofReloadTextures(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofReloadTextures(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofBackPressed(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBackPressed(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofOkPressed(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofOkPressed(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofCancelPressed(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofCancelPressed(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofGetFirstDrawOrder()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGetFirstDrawOrder();
}

void *ofxOfeliaEmbFunction::newWrapper_ofGetLastDrawOrder()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGetLastDrawOrder();
}

void *ofxOfeliaEmbFunction::newWrapper_ofTrueTypeFontSettings(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTrueTypeFontSettings(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofRequire(t_symbol *s)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofRequire(s);
}

void *ofxOfeliaEmbFunction::newWrapper_ofExpr(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofExpr(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofExpr_tilde(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofExpr_tilde(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofIf(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofIf(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofIf_tilde(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofIf_tilde(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofFor(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofFor(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTable(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTable(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper_ofGetDollarZero(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGetDollarZero(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofCanvasName(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofCanvasName(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofCanvasIndex(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofCanvasIndex(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofCanvasArgs(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofCanvasArgs(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofCanvasPosition(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofCanvasPosition(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofGetDir(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofGetDir(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofMakeFileName(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofMakeFileName(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofRemoveCanvas(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofRemoveCanvas(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofSine_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofSine_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofTriangle_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofTriangle_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofSaw_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofSaw_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofSquare_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofSquare_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofPulse_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofPulse_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofBlTriangle_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBlTriangle_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofBlSaw_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBlSaw_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofBlSquare_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBlSquare_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofBlPulse_tilde(t_floatarg f)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBlPulse_tilde(f);
}

void *ofxOfeliaEmbFunction::newWrapper_ofLowPass_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofLowPass_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofHighPass_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofHighPass_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofBandPass_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofBandPass_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofNotch_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofNotch_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofPeaking_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofPeaking_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofLowShelf_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofLowShelf_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofHighShelf_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofHighShelf_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofAllPass_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofAllPass_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofReverb_tilde()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofReverb_tilde();
}

void *ofxOfeliaEmbFunction::newWrapper_ofOscSender()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofOscSender();
}

void *ofxOfeliaEmbFunction::newWrapper_ofOscReceiver()
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod_ofOscReceiver();
}

void ofxOfeliaEmbFunction::setup()
{
    for (auto it = ofxOfeliaMaps::functionMap.begin(); it != ofxOfeliaMaps::functionMap.end(); ++it)
        class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_default),
                         gensym(it->first.c_str()), A_GIMME, 0);
    /* extras */
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofSetup),
                     gensym("ofSetup"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofUpdate),
                     gensym("ofUpdate"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofDraw),
                     gensym("ofDraw"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofExit),
                     gensym("ofExit"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofKeyPressed),
                     gensym("ofKeyPressed"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofKeyReleased),
                     gensym("ofKeyReleased"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseMoved),
                     gensym("ofMouseMoved"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseDragged),
                     gensym("ofMouseDragged"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMousePressed),
                     gensym("ofMousePressed"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseReleased),
                     gensym("ofMouseReleased"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseScrolled),
                     gensym("ofMouseScrolled"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseEntered),
                     gensym("ofMouseEntered"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMouseExited),
                     gensym("ofMouseExited"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofWindowResized),
                     gensym("ofWindowResized"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMessageReceived),
                     gensym("ofMessageReceived"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofDragged),
                     gensym("ofDragged"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTouchCancelled),
                     gensym("ofTouchCancelled"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTouchDoubleTap),
                     gensym("ofTouchDoubleTap"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTouchDown),
                     gensym("ofTouchDown"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTouchMoved),
                     gensym("ofTouchMoved"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTouchUp),
                     gensym("ofTouchUp"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofLostFocus),
                     gensym("ofLostFocus"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGotFocus),
                     gensym("ofGotFocus"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGotMemoryWarning),
                     gensym("ofGotMemoryWarning"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofDeviceOrientationChanged),
                     gensym("ofDeviceOrientationChanged"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofLaunchedWithURL),
                     gensym("ofLaunchedWithURL"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofSwipe),
                     gensym("ofSwipe"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofPause),
                     gensym("ofPause"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofStop),
                     gensym("ofStop"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofResume),
                     gensym("ofResume"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofReloadTextures),
                     gensym("ofReloadTextures"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBackPressed),
                     gensym("ofBackPressed"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofOkPressed),
                     gensym("ofOkPressed"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofCancelPressed),
                     gensym("ofCancelPressed"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGetFirstDrawOrder),
                     gensym("ofGetFirstDrawOrder"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGetLastDrawOrder),
                     gensym("ofGetLastDrawOrder"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTrueTypeFontSettings),
                     gensym("ofTrueTypeFontSettings"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofRequire),
                     gensym("ofRequire"), A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofExpr),
                     gensym("ofExpr"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofExpr_tilde),
                     gensym("ofExpr~"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofIf),
                     gensym("ofIf"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofIf_tilde),
                     gensym("ofIf~"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofFor),
                     gensym("ofFor"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTable),
                     gensym("ofTable"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGetDollarZero),
                     gensym("ofGetDollarZero"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofCanvasName),
                     gensym("ofCanvasName"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofCanvasIndex),
                     gensym("ofCanvasIndex"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofCanvasArgs),
                     gensym("ofCanvasArgs"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofCanvasPosition),
                     gensym("ofCanvasPosition"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofGetDir),
                     gensym("ofGetDir"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofMakeFileName),
                     gensym("ofMakeFileName"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofRemoveCanvas),
                     gensym("ofRemoveCanvas"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofSine_tilde),
                     gensym("ofSine~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofTriangle_tilde),
                     gensym("ofTriangle~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofSaw_tilde),
                     gensym("ofSaw~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofSquare_tilde),
                     gensym("ofSquare~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofPulse_tilde),
                     gensym("ofPulse~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBlTriangle_tilde),
                     gensym("ofBlTriangle~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBlSaw_tilde),
                     gensym("ofBlSaw~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBlSquare_tilde),
                     gensym("ofBlSquare~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBlPulse_tilde),
                     gensym("ofBlPulse~"), A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofLowPass_tilde),
                     gensym("ofLowPass~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofHighPass_tilde),
                     gensym("ofHighPass~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofBandPass_tilde),
                     gensym("ofBandPass~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofNotch_tilde),
                     gensym("ofNotch~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofPeaking_tilde),
                     gensym("ofPeaking~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofLowShelf_tilde),
                     gensym("ofLowShelf~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofHighShelf_tilde),
                     gensym("ofHighShelf~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofAllPass_tilde),
                     gensym("ofAllPass~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofReverb_tilde),
                     gensym("ofReverb~"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofOscSender),
                     gensym("ofOscSender"), A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_ofOscReceiver),
                     gensym("ofOscReceiver"), A_NULL, 0);
}
