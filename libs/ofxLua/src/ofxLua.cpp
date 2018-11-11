/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxLua for documentation
 *
 * This project uses the following library:
 *
 * Lua, Copyright (c) 1994â2011 Lua.org, PUC-Rio using the MIT License.
 * See the file "COPYRIGHT" in src/lua.
 * See http://www.lua.org/docs.html for documentation
 *
 */
#include "ofxLua.h"

#include "ofUtils.h"
#include "ofBindings.h"

// macro for chdir() as Windows uses a protected variant
#ifdef TARGET_WIN32
	#include <direct.h>
	#define CHDIR _chdir
#else
	#define CHDIR chdir
#endif
#include <errno.h>

// declare the wrapped modules
extern "C" {
	int luaopen_of(lua_State* L);
	int luaopen_glm(lua_State* L);
}

// local pointer for static functions
ofxLua *luaPtr = nullptr;

//------------------------------------------------------------------------------
ofxLua::ofxLua() {
	luaPtr = this;
	
	// make sure data path is absolute in case we change to the current script dir
	ofSetDataPathRoot(ofFilePath::getAbsolutePath(ofToDataPath("")));
}

//------------------------------------------------------------------------------
ofxLua::~ofxLua() {
	clear();
}

//------------------------------------------------------------------------------
bool ofxLua::init(bool abortOnError, bool openLibs, bool ofBindings) {
	
	clear();
	
	L = luaL_newstate();
	if(L == NULL) {
		ofLogError("ofxLua") << "Error initializing lua";
		return false;
	}
	
	// open libs
	if(openLibs) {
		luaL_openlibs(L);
	}
	if(ofBindings) {
		luaopen_of(L);
		luaopen_glm(L);
	}
	
	// clear stack since opening libs leaves tables on the stack
	lua_settop(L, 0);
	
	// set the panic function
	lua_atpanic(L, &atPanic);
	
	this->abortOnError = abortOnError;
	ofLogVerbose("ofxLua") << "Initialized state";
	
	return true;
}

void ofxLua::clear() {
	if(L != NULL) {
		lua_close(L);
		L = NULL;
		ofLogVerbose("ofxLua") << "Cleared state";
	}
	tables.clear();
	errorMessage = "";
}

bool ofxLua::isValid() {
	return (L != NULL);
}

bool ofxLua::getAbortOnError() {
	return abortOnError;
}

void ofxLua::setAbortOnError(bool abort) {
	abortOnError = abort;
}

//------------------------------------------------------------------------------
bool ofxLua::doString(const std::string& text) {
	
	if(!isValid()) {
		ofLogError("ofxLua") << "Cannot do std::string, lua state not inited!";
		return false;
	}

	if(text.length() < 10) {
		ofLogVerbose("ofxLua") << "Doing std::string: \"" << text << "\"";
	}
	else {
		ofLogVerbose("ofxLua") << "Doing std::string: \"" << text.substr(0,10) << "..." << "\"";
	}
	
	// load the std::string
	int ret = luaL_loadstring(L, text.c_str());
	if(ret != 0) {
		switch(ret) {
			case LUA_ERRSYNTAX: {
				std::string msg = (std::string) lua_tostring(L, LUA_STACK_TOP);
				errorOccurred(msg);
				break;
			}
			case LUA_ERRMEM: {
				std::string msg = "Memory error",
				errorOccurred(msg);
				break;
			}
		}
				
		return false;
	}
	
	// run the std::string
	ret = lua_pcall(L, 0, LUA_MULTRET, 0);
	if(ret != 0) {
		std::string msg = (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
		return false;
	}
	
	return true;
}

//------------------------------------------------------------------------------
bool ofxLua::doScript(const std::string& script, bool changeDir) {

	if(!isValid()) {
		ofLogError("ofxLua") << "Cannot do script, lua state not inited!";
		return false;
	}
	
	std::string fullpath = ofFilePath::getAbsolutePath(ofToDataPath(script));
	std::string file = ofFilePath::getFileName(fullpath);
	std::string folder = ofFilePath::getEnclosingDirectory(fullpath);
	
	// trim trailing slash
	if(folder.size() > 0 && folder.at(folder.size()-1) == '/') {
		folder.erase(folder.end()-1);
	}
	
	ofLogVerbose("ofxLua") << "Doing script: \"" << file << "\" path: \"" << folder << "\"";
	if(changeDir) {
		ofLogVerbose("ofxLua") << "Changing to script dir \"" << folder << "\"";
		if(CHDIR(folder.c_str()) < 0) {
			switch(errno) {
				case ENOENT:
					ofLogError("ofxLua") << "Script dir \"" << folder << "\" does not exist";
					break;
				case EACCES:
					ofLogError("ofxLua") << "Could not access script dir \"" << folder << "\"";
					break;
				default:
					ofLogError("ofxLua") << "Could not change to script dir \"" << folder << "\", error " << errno;
					break;
			}
		}
	}

	// load the script
	int ret = luaL_loadfile(L, fullpath.c_str());
	if(ret != 0) {
		switch(ret) {
			case LUA_ERRFILE: {
				std::string msg = (std::string)"Script \""+file+"\" not found or unreadable";
				errorOccurred(msg);
				break;
			}
			case LUA_ERRSYNTAX: {
				std::string msg = (std::string) lua_tostring(L, LUA_STACK_TOP);
				errorOccurred(msg);
				break;
			}
			case LUA_ERRMEM: {
				std::string msg = "Memory error for script \""+file+"\"";
				errorOccurred(msg);
				break;
			}
		}
		return false;
	}
	
	// run the script
	if(lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
		std::string msg = (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
		return false;
	}
	
	return true;
}

//------------------------------------------------------------------------------		
void ofxLua::addListener(ofxLuaListener* listener) {
	ofAddListener(errorEvent, listener, &ofxLuaListener::errorReceived);
}
		
//------------------------------------------------------------------------------		
void ofxLua::removeListener(ofxLuaListener* listener) {
	ofRemoveListener(errorEvent, listener, &ofxLuaListener::errorReceived);
}

//------------------------------------------------------------------------------
void ofxLua::setErrorCallback(std::function<void(std::string& message)> const &callback) {
	errorCallback = callback;
}

//--------------------------------------------------------------------
void ofxLua::scriptSetup() {
	if(L == NULL || !isFunction("setup")) {
		return;
	}
	lua_getglobal(L, "setup");
	if(lua_pcall(L, 0, 0, 0) != 0) {
		std::string msg = "Error running setup(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptUpdate() {
	if(L == NULL || !isFunction("update")) {
		return;
	}
	lua_getglobal(L, "update");
	if(lua_pcall(L, 0, 0, 0) != 0) {
		std::string msg = "Error running update(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptDraw() {
	if(L == NULL || !isFunction("draw")) {
		return;
	}
	lua_getglobal(L, "draw");
	if(lua_pcall(L, 0, 0, 0) != 0) {			
		std::string msg = "Error running draw(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptExit() {
	if(L == NULL || !isFunction("exit")) {
		return;
	}
	lua_getglobal(L, "exit");
	if(lua_pcall(L, 0, 0, 0) != 0) {
		std::string msg = "Error running exit(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

//--------------------------------------------------------------------
void ofxLua::scriptWindowResized(int w, int h) {
	if(L == NULL || !isFunction("windowResized")) {
		return;
	}
	lua_getglobal(L, "windowResized");
	lua_pushinteger(L, w);
	lua_pushinteger(L, h);
	if(lua_pcall(L, 2, 0, 0) != 0) {
		std::string msg = "Error running windowResized(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptKeyPressed(int key) {
	if(L == NULL || !isFunction("keyPressed")) {
		return;
	}
	lua_getglobal(L, "keyPressed");
	lua_pushinteger(L, key);
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running keyPressed(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptKeyReleased(int key) {
	if(L == NULL || !isFunction("keyReleased")) {
		return;
	}
	lua_getglobal(L, "keyReleased");
	lua_pushinteger(L, key);
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running keyReleased(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseMoved(int x, int y ) {
	if(L == NULL || !isFunction("mouseMoved")) {
		return;
	}
	lua_getglobal(L, "mouseMoved");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	if(lua_pcall(L, 2, 0, 0) != 0) {
		std::string msg = "Error running mouseMoved(): "
		                  + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseDragged(int x, int y, int button) {
	if(L == NULL || !isFunction("mouseDragged")) {
		return;
	}
	lua_getglobal(L, "mouseDragged");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	lua_pushinteger(L, button);
	if(lua_pcall(L, 3, 0, 0) != 0) {
		std::string msg = "Error running mouseDragged(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMousePressed(int x, int y, int button) {
	if(L == NULL || !isFunction("mousePressed")) {
		return;
	}
	lua_getglobal(L, "mousePressed");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	lua_pushinteger(L, button);
	if(lua_pcall(L, 3, 0, 0) != 0) {			
		std::string msg = "Error running mousePressed(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseReleased(int x, int y, int button) {
	if(L == NULL || !isFunction("mouseReleased")) {
		return;
	}
	lua_getglobal(L, "mouseReleased");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	lua_pushinteger(L, button);
	if(lua_pcall(L, 3, 0, 0) != 0) {
		std::string msg = "Error running mouseReleased(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseScrolled(int x, int y, float scrollX, float scrollY) {
	if(L == NULL || !isFunction("mouseScrolled")) {
		return;
	}
	lua_getglobal(L, "mouseScrolled");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	lua_pushnumber(L, scrollX);
	lua_pushnumber(L, scrollY);
	if(lua_pcall(L, 4, 0, 0) != 0) {
		std::string msg = "Error running mouseScrolled(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseEntered(int x, int y) {
	if(L == NULL || !isFunction("mouseEntered")) {
		return;
	}
	lua_getglobal(L, "mouseEntered");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	if(lua_pcall(L, 2, 0, 0) != 0) {
		std::string msg = "Error running mouseEntered(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptMouseExited(int x, int y) {
	if(L == NULL || !isFunction("mouseExited")) {
		return;
	}
	lua_getglobal(L, "mouseExited");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	if(lua_pcall(L, 2, 0, 0) != 0) {
		std::string msg = "Error running mouseExited(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

//--------------------------------------------------------------
void ofxLua::scriptDragEvent(ofDragInfo dragInfo) {
	if(L == NULL || !isFunction("dragEvent")) {
		return;
	}
	lua_getglobal(L, "dragEvent");
	pushobject("ofDragInfo", new ofDragInfo(dragInfo)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running dragInfo(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptGotMessage(ofMessage msg) {
	if(L == NULL || !isFunction("gotMessage")) {
		return;
	}
	lua_getglobal(L, "gotMessage");
	lua_pushstring(L, msg.message.c_str());
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running gotMessage(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

//--------------------------------------------------------------
void ofxLua::scriptTouchDown(ofTouchEventArgs &touch) {
	if(L == NULL || !isFunction("touchDown")) {
		return;
	}
	lua_getglobal(L, "touchDown");
	pushobject("ofTouchEventArgs", new ofTouchEventArgs(touch)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running touchDown(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptTouchMoved(ofTouchEventArgs &touch) {
	if(L == NULL || !isFunction("touchMoved")) {
		return;
	}
	lua_getglobal(L, "touchMoved");
	pushobject("ofTouchEventArgs", new ofTouchEventArgs(touch)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running touchMoved(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptTouchUp(ofTouchEventArgs &touch) {
	if(L == NULL || !isFunction("touchUp")) {
		return;
	}
	lua_getglobal(L, "touchUp");
	pushobject("ofTouchEventArgs", new ofTouchEventArgs(touch)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running touchUp(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptTouchDoubleTap(ofTouchEventArgs &touch) {
	if(L == NULL || !isFunction("touchDoubleTap")) {
		return;
	}
	lua_getglobal(L, "touchDoubleTap");
	pushobject("ofTouchEventArgs", new ofTouchEventArgs(touch)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running touchDoubleTap(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

void ofxLua::scriptTouchCancelled(ofTouchEventArgs &touch) {
	if(L == NULL || !isFunction("touchCancelled")) {
		return;
	}
	lua_getglobal(L, "touchCancelled");
	pushobject("ofTouchEventArgs", new ofTouchEventArgs(touch)); // lua manages this memory
	if(lua_pcall(L, 1, 0, 0) != 0) {
		std::string msg = "Error running touchCancelled(): "
					 + (std::string) lua_tostring(L, LUA_STACK_TOP);
		errorOccurred(msg);
	}
}

//------------------------------------------------------------------------------
bool ofxLua::isBool(const std::string& name) {
	return exists(name, LUA_TBOOLEAN);
}

bool ofxLua::isBool(const unsigned int index) {
	return exists(index, LUA_TBOOLEAN);
}

bool ofxLua::isNumber(const std::string& name) {
	return exists(name, LUA_TNUMBER);
}

bool ofxLua::isNumber(const unsigned int index) {
	return exists(index, LUA_TNUMBER);
}

bool ofxLua::isString(const std::string& name) {
	return exists(name, LUA_TSTRING);
}

bool ofxLua::isString(const unsigned int index) {
	return exists(index, LUA_TSTRING);
}

bool ofxLua::isFunction(const std::string& name) {
	return exists(name, LUA_TFUNCTION);
}

bool ofxLua::isFunction(const unsigned int index) {
	return exists(index, LUA_TFUNCTION);
}

bool ofxLua::isTable(const std::string& name) {
	return exists(name, LUA_TTABLE);
}

bool ofxLua::isTable(const unsigned int index) {
	return exists(index, LUA_TTABLE);
}

bool ofxLua::isNil(const std::string& name) {
	return exists(name, LUA_TNIL);
}

bool ofxLua::isNil(const unsigned int index) {
	return exists(index, LUA_TNIL);
}
		
//------------------------------------------------------------------------------
void ofxLua::newTable(const std::string& tableName) {
	if(!isValid()) {
		return;
	}
	
	// global table
	if(tables.empty()) {
		lua_newtable(L);
		lua_setglobal(L, tableName.c_str());
	}
	
	// table in another table
	else {
		lua_pushstring(L, tableName.c_str()); // key
		lua_newtable(L); // value
		lua_settable(L, -3);
	}
}

void ofxLua::newTable(const unsigned int& tableIndex) {
	if(!isValid()) {
		return;
	}
	
	// global table
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Cannot create new global table by index" << tableIndex;
		return;
	}
	
	lua_pushinteger(L, tableIndex); // index
	lua_newtable(L); // value
	lua_settable(L, -3);
}

bool ofxLua::pushTable(const std::string& tableName) {
	if(!isValid()) {
		return false;
	}
	
	// global table
	if(tables.empty()) {
		lua_getglobal(L, tableName.c_str());
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't push global table \"" << tableName << "\"";
			return false;
		}
	}
	
	// table in another table
	else {
		lua_pushstring(L, tableName.c_str());
		lua_gettable(L, -2);
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't push table \"" << tableName << "\"";
			return false;
		}
	}
	
	tables.push_back({LUA_TSTRING, tableName, 0});
	return true;
}

bool ofxLua::pushTable(const unsigned int& tableIndex) {
	if(!isValid()) {
		return false;
	}

	// global table
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Cannot push global table by index" << tableIndex;
		return false;
	}
	
	lua_pushinteger(L, tableIndex);
	lua_gettable(L, -2);
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't push table \"" << tableIndex << "\"";
		return false;
	}
	tables.push_back({LUA_TNUMBER, "", (unsigned int)tableIndex});
	
	return true;
}

void ofxLua::popTable() {
	if(!isValid()) {
		return;
	}

	if(tables.empty()) {
		ofLogWarning("ofxLua") << "No tables to pop, did you push?";
		return;
	}
	tables.pop_back();
	lua_pop(L, 1);
}

void ofxLua::popAllTables() {
	if(!isValid()) {
		return;
	}

	while(!tables.empty()) {
		popTable();
	}
	lua_settop(L, 0); // clear stack just in case
}

unsigned int ofxLua::tableSize() {
	if(!isValid()) {
		return 0;
	}
	
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't get table size, no open tables";
		return 0;
	}
	
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't get table size, stack var is not a table";
		return 0;
	}
	
	return lua_rawlen(L, LUA_STACK_TOP);
}

unsigned int ofxLua::tableSize(const std::string& tableName) {
	unsigned int size = 0;
	pushTable(tableName);
	size = tableSize();
	popTable();
	return size;
}

unsigned int ofxLua::tableSize(const unsigned int& tableIndex) {
	unsigned int size = 0;
	pushTable(tableIndex);
	size = tableSize();
	popTable();
	return size;
}

void ofxLua::printTable() {
	if(!isValid()) {
		return;
	}

	if(tables.empty()) {
		ofLogNotice("ofxLua") << "global table";
		lua_pushglobaltable(L);
		printTable(LUA_STACK_TOP, 1);
		lua_pop(L, 1);
		return;
	}
	
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't print table, stack var is not a table";
		return;
	}
	
	ofLogNotice("ofxLua") <<  "table " << (std::string)tables.back();
	printTable(LUA_STACK_TOP, 1);
}

void ofxLua::printTable(const std::string& tableName) {
	if(!pushTable(tableName)) {
		return;
	}
	printTable();
	popTable();
}

void ofxLua::printTable(const unsigned int& tableIndex) {
	if(!pushTable(tableIndex)) {
		return;
	}
	printTable();
	popTable();
}

void ofxLua::clearTable() {
	if(!isValid()) {
		return;
	}
	
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "No table to clear, did you push?";
		return;
	}
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't clear table, stack var is not a table";
	}
	
	// clears all elements by setting them to nil, this way the original table
	// pointer is preserved
	lua_pushvalue(L, LUA_STACK_TOP); // stack: -1 => table
	lua_pushnil(L); // stack: -1 => table; -2 => nil
	
	while(lua_next(L, -2)) {
		// stack: -3 => table; -2 => key; -1 => value
		lua_pop(L, 1); // stack: -2 => table; -1 => key
		
		lua_pushvalue(L, -1);
		lua_pushnil(L);
		lua_settable(L, -4); // stack: -2 => table; -1 => key
	}
	
	// stack: -1 => table
	lua_pop(L, 1); // stack:
}

void ofxLua::clearTable(const std::string& tableName) {
	if(!pushTable(tableName)) {
		return;
	}
	clearTable();
	popTable();
}

void ofxLua::clearTable(const unsigned int& tableIndex) {
	if(!pushTable(tableIndex)) {
		return;
	}
	clearTable();
	popTable();
}
		
//------------------------------------------------------------------------------
bool ofxLua::getBool(const std::string& name, bool defaultValue) {
	return read<bool>(name, LUA_TBOOLEAN, defaultValue);
}

bool ofxLua::getBool(const unsigned int index, bool defaultValue) {
	return read<bool>(index, LUA_TBOOLEAN, defaultValue);
}

lua_Number ofxLua::getNumber(const std::string& name, lua_Number defaultValue) {
	return read<lua_Number>(name, LUA_TNUMBER, defaultValue);
}

lua_Number ofxLua::getNumber(const unsigned int index, lua_Number defaultValue) {
	return read<lua_Number>(index, LUA_TNUMBER, defaultValue);
}

std::string ofxLua::getString(const std::string& name, const std::string& defaultValue) {
	return read<std::string>(name, LUA_TSTRING, defaultValue);
}

std::string ofxLua::getString(const unsigned int index, const std::string& defaultValue) {
	return read<std::string>(index, LUA_TSTRING, defaultValue);
}

void ofxLua::getBoolVector(const std::string& tableName, std::vector<bool>& v) {
	readVector<bool>(tableName, v, LUA_TBOOLEAN, false);
}

void ofxLua::getBoolVector(const unsigned int tableIndex, std::vector<bool>& v) {
	readVector<bool>(tableIndex, v, LUA_TBOOLEAN, false);
}

void ofxLua::getNumberVector(const std::string& tableName, std::vector<lua_Number>& v) {
	readVector<lua_Number>(tableName, v, LUA_TNUMBER, 0.0f);
}

void ofxLua::getNumberVector(const unsigned int tableIndex, std::vector<lua_Number>& v) {
	readVector<lua_Number>(tableIndex, v, LUA_TNUMBER, 0.0f);
}

void ofxLua::getStringVector(const std::string& tableName, std::vector<std::string>& v) {
	readVector<std::string>(tableName, v, LUA_TSTRING, "");
}

void ofxLua::getStringVector(const unsigned int tableIndex, std::vector<std::string>& v) {
	readVector<std::string>(tableIndex, v, LUA_TSTRING, "");
}

//------------------------------------------------------------------------------
void ofxLua::setBool(const std::string& name, bool value) {
	write<bool>(name, LUA_TBOOLEAN, value);
}

void ofxLua::setBool(const unsigned int index, bool value) {
	write<bool>(index, LUA_TBOOLEAN, value);
}

void ofxLua::setNumber(const std::string& name, lua_Number value) {
	write<lua_Number>(name, LUA_TNUMBER, value);
}

void ofxLua::setNumber(const unsigned int index, lua_Number value) {
	write<lua_Number>(index, LUA_TNUMBER, value);
}

void ofxLua::setString(const std::string& name, const std::string value) {
	write<std::string>(name, LUA_TSTRING, value);
}

void ofxLua::setString(const unsigned int index, const std::string value) {
	write<std::string>(index, LUA_TSTRING, value);
}

void ofxLua::setBoolVector(const std::string& tableName, std::vector<bool>& v) {
	writeVector<bool>(tableName, LUA_TBOOLEAN, v);
}

void ofxLua::setBoolVector(const unsigned int tableIndex, std::vector<bool>& v) {
	writeVector<bool>(tableIndex, LUA_TBOOLEAN, v);
}

void ofxLua::setNumberVector(const std::string& tableName, std::vector<lua_Number>& v) {
	writeVector<lua_Number>(tableName, LUA_TNUMBER, v);
}

void ofxLua::setNumberVector(const unsigned int tableIndex, std::vector<lua_Number>& v) {
	writeVector<lua_Number>(tableIndex, LUA_TNUMBER, v);
}

void ofxLua::setStringVector(const std::string& tableName, std::vector<std::string>& v) {
	writeVector<std::string>(tableName, LUA_TSTRING, v);
}

void ofxLua::setStringVector(const unsigned int tableIndex, std::vector<std::string>& v) {
	writeVector<std::string>(tableIndex, LUA_TSTRING, v);
}

void ofxLua::setNil(const std::string& name) {
	if(!isValid()) {
		return;
	}

	// global variable?
	if(tables.empty()) {
		lua_pushglobaltable(L);
		lua_pushnil(L);
		lua_setfield(L, -2, name.c_str());
		lua_pop(L, 1);
	}
	
	// in a table namespace
	else {
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't set unknown var \"" << name << "\" to nil";
			return;
		}
		lua_pushstring(L, name.c_str());
		lua_pushnil(L);
		lua_settable(L, -3);
	}
}

void ofxLua::setNil(const unsigned int index) {
	if(!isValid()) {
		return;
	}
	
	// global variable?
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write global var by index, no open tables";
		return;
	}
	
	// in a table namespace
	else {
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't set unknown index " << index << " to nil";
			return;
		}
		lua_pushinteger(L, index);
		lua_pushnil(L);
		lua_settable(L, -3);
	}
}

//------------------------------------------------------------------------------
void ofxLua::writeTable(ofxLuaFileWriter& writer, bool recursive) {
	if(!isValid()) {
		return;
	}

	if(tables.empty()) {
		ofLogWarning("ofxLua") << "No table to write, did you push?";
		return;
	}
	
	// in a table namespace
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't write table \"" << (std::string)tables.back()
			<< "\", top of stack is not a table";
		return;
	}
	
	writeTable(LUA_STACK_TOP, writer, recursive);
}

void ofxLua::writeTable(const std::string& tableName, ofxLuaFileWriter& writer, bool recursive) {
	if(!pushTable(tableName)) {
		return;
	}
	writer.beginTable(tableName);
	writeTable(writer, recursive);
	writer.endTable();
	popTable();
}

bool ofxLua::writeTableToFile(const std::string& filename, bool recursive) {
	ofxLuaFileWriter writer;
	writeTable(writer, recursive);
	return writer.saveToFile(filename);
}

bool ofxLua::writeTableToFile(const std::string& tableName, const std::string& filename, bool recursive) {
	ofxLuaFileWriter writer;
	writeTable(tableName, writer, recursive);
	return writer.saveToFile(filename);
}

//------------------------------------------------------------------------------
void ofxLua::errorOccurred(std::string& message) {
	
	errorMessage = message;
	
	// send to listeners
	ofNotifyEvent(errorEvent, message, this);
	if(errorCallback) {
		errorCallback(message);
	}
	
	// close the state?
	if(abortOnError) {
		clear();
	}
}

// from http://www.lua.org/pil/24.2.3.html
void ofxLua::printStack() {
	if(!isValid()) {
		return;
	}
	
	std::stringstream line;
	line << "stack " << lua_gettop(L);

	int top = lua_gettop(L);
	if(top > 0) {
		line << ":";
	}
	for(int i = 1; i <= top; i++) {
		int type = lua_type(L, i);
		switch(type) {

			case LUA_TSTRING:
				line << " \"" << lua_tostring(L, i) << "\"";
				break;

			case LUA_TBOOLEAN:
				line << " " << (lua_toboolean(L, i) == 1 ? "true" : "false");
				break;

			case LUA_TNUMBER:
				line << " " << lua_tonumber(L, i);
				break;

			default:
				line << " " << lua_typename(L, type);
				break;

		}
		if(i != top) {
			line << " ";
		}
	}
	ofLogNotice("ofxLua") << line.str();
}

// push object pointer to Lua using SWIG helper function,
// from http://stackoverflow.com/questions/9455552/swiglua-passing-a-c-instance-as-a-lua-function-parameter
bool ofxLua::pushobject(const std::string &typeName, void *object, bool manageMemory) {
	std::string typeString = typeName + " *";
	swig_type_info *type = SWIG_TypeQuery(L, typeString.c_str());
	if(type == NULL) {
		return false;
	}
	SWIG_NewPointerObj(L, object, type, manageMemory);
	return true;
}

// PRIVATE

//------------------------------------------------------------------------------
// type specific implementations
template<> bool ofxLua::totype<bool>(int stackIndex, int type, bool defaultValue) {
	if(lua_type(L, stackIndex) != type) {
		return defaultValue;
	}
	switch(type) {
		case LUA_TBOOLEAN:
			return lua_toboolean(L, stackIndex);
		default:
			return defaultValue;
	}
}

template<> lua_Number ofxLua::totype<lua_Number>(int stackIndex, int type, lua_Number defaultValue) {
	if(lua_type(L, stackIndex) != type) {
		return defaultValue;
	}
	switch(type) {
		case LUA_TNUMBER:
			return lua_tonumber(L, stackIndex);
		default:
			return defaultValue;
	}
}

template<> std::string ofxLua::totype<std::string>(int stackIndex, int type, std::string defaultValue) {
	if(lua_type(L, stackIndex) != type) {
		return defaultValue;
	}
	switch(type) {
		case LUA_TSTRING:
			return lua_tostring(L, stackIndex);
		default:
			return defaultValue;
	}
}

//------------------------------------------------------------------------------
template <> void ofxLua::settype<bool>(const std::string& name, int type, bool value) {
	if(type == LUA_TBOOLEAN) {
		lua_pushboolean(L, value);
		lua_setfield(L, -2, name.c_str());
	}
}

template <> void ofxLua::settype<bool>(unsigned int index, int type, bool value) {
	if(type == LUA_TBOOLEAN) {
		lua_pushinteger(L, index);
		lua_pushboolean(L, value);
		lua_settable(L, -3);
	}
}

template <> void ofxLua::settype<lua_Number>(const std::string& name, int type, lua_Number value) {
	if(type == LUA_TNUMBER) {
		lua_pushnumber(L, value);
		lua_setfield(L, -2, name.c_str());
	}
}

template <> void ofxLua::settype<lua_Number>(unsigned int index, int type, lua_Number value) {
	if(type == LUA_TNUMBER) {
		lua_pushinteger(L, index);
		lua_pushnumber(L, value);
		lua_settable(L, -3);
	}
}

template <> void ofxLua::settype<std::string>(const std::string& name, int type, std::string value) {
	if(type == LUA_TSTRING) {
		lua_pushstring(L, value.c_str());
		lua_setfield(L, -2, name.c_str());
	}
}

template <> void ofxLua::settype<std::string>(unsigned int index, int type, std::string value) {
	if(type == LUA_TSTRING) {
		lua_pushinteger(L, index);
		lua_pushstring(L, value.c_str());
		lua_settable(L, -3);
	}
}

//------------------------------------------------------------------------------
bool ofxLua::exists(const std::string& name, int type) {
	if(!isValid()) {
		return false;
	}

	// global variable
	bool ret = false;
	if(tables.empty()) {
		lua_getglobal(L, name.c_str());
		ret = checkType(LUA_STACK_TOP, type);
		lua_pop(L, 1);
	}
	
	// table variable
	else {
		lua_getfield(L, LUA_STACK_TOP, name.c_str());
		if(!lua_istable(L, LUA_STACK_TOP-1)) {
			ofLogWarning("ofxLua") << "Couldn't check existence of \"" << name
				<< "\", top of stack is not a table";
			ret = false;
		}
		else {
			ret = checkType(LUA_STACK_TOP, type);
		}
		lua_pop(L, 1);
	}
	return ret;
}

//------------------------------------------------------------------------------
bool ofxLua::exists(const unsigned int index, int type) {
	if(!isValid()) {
		return false;
	}
	
	bool ret = false;
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Cannot check existence of global var by index " << index;
		ofLogWarning("ofxLua") << "Did you forget to push a table? ";
		return false;
	}
	
	// table variable
	else {
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't check existence of index " << index
				<< ", top of stack is not a table";
			ret = false;
		}
		else {
			lua_pushinteger(L, index);
			lua_gettable(L, -2);
			ret = checkType(LUA_STACK_TOP, type);
			lua_pop(L, 1);
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
bool ofxLua::checkType(int stackIndex, int type) {
	return lua_type(L, stackIndex) == type;
}

//------------------------------------------------------------------------------
// from http://stackoverflow.com/questions/6137684/iterate-through-lua-table
void ofxLua::printTable(int stackIndex, int numTabs) {
	
	std::string tabs;
	for(int i = 0; i < numTabs; ++i) {
		tabs += "\t";
	}
	
	lua_pushvalue(L, stackIndex); // stack: -1 => table
	lua_pushnil(L); // stack : -2 => table; -1 => nil
	
	std::stringstream line;
	while(lua_next(L, -2)) {
	
		// stack: -3 => table; -2 => key; -1 => value
		lua_pushvalue(L, -2);
		// stack: -4 => table; -3 => key; -2 => value; -1 => key
	
		// ignore global, packages, etc
		std::string name = (std::string) lua_tostring(L, -1);
		if(name == "_G" || name == "package") {
			line.str("");
			lua_pop(L, 2);
			continue;
		}
	
		// print value type and key
		line << tabs << (std::string) lua_typename(L, lua_type(L, -2)) << " " << name;
		
		// recurse if a table
		if(lua_istable(L, -2)) {
			ofLogNotice("ofxLua") << line.str();
			printTable(-2, numTabs+1);
		}
		else { // print values
			switch(lua_type(L, -2)) {
				case LUA_TBOOLEAN:
					ofLogNotice("ofxLua") << line.str() << ": " << ((bool)lua_toboolean(L, -2) ? "true" : "false");
					break;
				case LUA_TNUMBER:
					ofLogNotice("ofxLua") << line.str() << ": " << lua_tonumber(L, -2);
					break;
				case LUA_TSTRING:
					ofLogNotice("ofxLua") << line.str() << ": \"" << lua_tostring(L, -2) << "\"";
					break;
				default:
					ofLogNotice("ofxLua") << line.str();
					break;
			}
		}
		line.str("");
		lua_pop(L, 2); // stack: -2 => table; -1 => key
	}
	
	// stack: -1 => table
	lua_pop(L, 1); // stack:
}

//------------------------------------------------------------------------------
void ofxLua::writeTable(int stackIndex, ofxLuaFileWriter& writer, bool recursive) {
	
	lua_pushvalue(L, stackIndex); // stack: -1 => table
	lua_pushnil(L); // stack: -2 => table; -1 => nil
	
	std::stringstream line;
	while(lua_next(L, -2)) {

		// stack: -3 => table; -2 => key; -1 => value
		lua_pushvalue(L, -2);
		// stack: -4 => table; -3 => key; -2 => value; -1 => key
	
		// recurse if a table
		if(lua_istable(L, -2)) {
			if(recursive) {
				if(lua_isnumber(L, -1)) {
					writer.beginTable((int)lua_tonumber(L, -1));
				}
				else {
					writer.beginTable(lua_tostring(L, -1));
				}
				writeTable(-2, writer, true);
				writer.endTable();
			}
		}
		else { // print write values based on the key type
			switch(lua_type(L, -2)) {
				case LUA_TBOOLEAN:
					if(lua_isnumber(L, -1)) {
						writer.writeBool(lua_tonumber(L, -1), (bool)lua_toboolean(L, -2));
					}
					else if(lua_isstring(L, -1)) {
						writer.writeBool((std::string) lua_tostring(L, -1), (bool)lua_toboolean(L, -2));
					}
					else {
						ofLogWarning("ofxLua") << "Unknown key type when writing table";
					}
					break;
				case LUA_TNUMBER:
					if(lua_isnumber(L, -1)) {
						writer.writeNumber(lua_tonumber(L, -1), lua_tonumber(L, -2));
					}
					else if(lua_isstring(L, -1)) {
						writer.writeNumber((std::string) lua_tostring(L, -1), lua_tonumber(L, -2));
					}
					else {
						ofLogWarning("ofxLua") << "Unknown key type when writing table";
					}
					break;
				case LUA_TSTRING:
					if(lua_isnumber(L, -1)) {
						writer.writeString(lua_tonumber(L, -1), lua_tostring(L, -2));
					}
					else if(lua_isstring(L, -1)) {
						writer.writeString((std::string) lua_tostring(L, -1), lua_tostring(L, -2));
					}
					else {
						ofLogWarning("ofxLua") << "Unknown key type when writing table";
					}
					break;
				default:
					break;
			}
		}
		line.str("");
		lua_pop(L, 2); // stack: -2 => table; -1 => key
	}
	
	// stack: -1 => table
	lua_pop(L, 1); // stack:
}

//------------------------------------------------------------------------------
int ofxLua::atPanic(lua_State *L) {
	ofLogError("ofxLua") << "Lua panic ocurred! : " << lua_tostring(L, LUA_STACK_TOP);
	ofLogError("ofxLua") << "Closing state";
	luaPtr->clear();
	return 0;
}
