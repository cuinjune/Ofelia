// events folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofEvent.h -----

// not needed

// ----- ofEvents.h -----

// DIFF: ofEvents.h:
// DIFF:   ignore event classes, event args, and registration functions
%ignore ofEventArgs;
%ignore ofEntryEventArgs;
%ignore ofKeyEventArgs;
%ignore ofMouseEventArgs;
// need ofTouchEventArgs for touch callbacks
%ignore ofAudioEventArgs;
%ignore ofResizeEventArgs;
%ignore ofWindowPosEventArgs;
%ignore ofMessage;
%ignore ofTimeMode;

%ignore ofCoreEvents;

// DIFF:   ignore ofSendMessage() with ofMessage in favor of std::string
%ignore ofSendMessage(ofMessage msg);

%ignore ofEvents;

%ignore ofRegisterMouseEvents;
%ignore ofRegisterKeyEvents;
%ignore ofRegisterTouchEvents;
%ignore ofRegisterGetMessages;
%ignore ofRegisterDragEvents;
%ignore ofUnregisterMouseEvents;
%ignore ofUnregisterKeyEvents;
%ignore ofUnregisterTouchEvents;
%ignore ofUnregisterGetMessages;
%ignore ofUnregisterDragEvents;

%include "events/ofEvents.h"

// DIFF:   added target language tostring wrapper for ofTouchEventArgs::operator <<
// TODO:   ofTouchEventArgs inheritance from glm::vec2 doesn't create x & y attributes
%extend ofTouchEventArgs {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// manually create attributes since inherting from
// glm::vec2 doesn't seem to create them
%attributeVar(ofTouchEventArgs, float, x, x, x);
%attributeVar(ofTouchEventArgs, float, y, y, y);

// ----- ofEventUtils.h -----

// not needed
