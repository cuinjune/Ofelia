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
#pragma once

#include "ofConstants.h"
#include "ofEvents.h"

#include "lua.hpp"
#include "ofxLuaFileWriter.h"

// define pua_pushglobaltable for Lua 5.1 api
#if LUA_VERSION_NUM < 502
	#define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX)
#endif

/// a baseclass to receieve Lua error messages,
/// useful for displaying an error message on the screen, etc
class ofxLuaListener {
	public :
		virtual void errorReceived(std::string& message) = 0;
};

///	a Lua interpreter instance
///
///	references:
///     - Lua api http://www.lua.org/manual/5.1/manual.html
///		- SWIG and Lua http://swig.org/Doc1.3/Lua.html
///
/// read/write algos are largely derived from the Allacrost scripting system:
/// http://allacrost.sourceforge.net/
class ofxLua {
	
	public :

		ofxLua();
		virtual ~ofxLua();
		
		/// init the Lua state
		///
		/// set abortOnError to true if you want the Lua state to be cleared
		/// when an error occurs
		///
		/// set openLibs to false if you don't want to load the standard Lua
		/// libs (math, io, std::string, etc)
		///
		/// set ofBindings to false if you don't want to load the openFrameworks
		/// api Lua bindings
		///
		/// note: calls clear if the state has already been inited
		bool init(bool abortOnError=false, bool openLibs=true, bool ofBindings=true);
		
		/// clears current state
		///
		/// note: this also clears all loaded modules
		void clear();
		
		/// is this state valid? (inited, not aborted, etc)
		bool isValid();
		
		/// get abort on error
		/// if abort on error is true, the state is closed when an error ocurrs
		bool getAbortOnError();

		/// set abort on error
		/// if abort on error is true, the state is closed when an error ocurrs
		void setAbortOnError(bool abort);
		
	/// \section Running Lua code
		
		/// run a Lua script string, returns false on script error
		bool doString(const std::string& text);
		
		/// run a Lua script, returns false on script error
		///
		/// set changeDir = true to change the current working dir
		/// to the script's parent dir, use this if Lua's require command
		/// cannot find local scripts
		///
		/// note: changeDir does not affect the current OF data path
		bool doScript(const std::string& script, bool changeDir=false);
		
	/// \section Listeners
		
		/// add a listener, ignores any duplicates
		void addListener(ofxLuaListener* listener);
		
		/// remove a listener
		void removeListener(ofxLuaListener* listener);

		/// set the error callback function directly as an altervative to using
		/// a listener, clear by setting to nulltpr
		///
		/// example:
		///
		///    lua.setErrorCallback([](std::string& message) {
		///         // this is called when a Lua script error occurrs
		///        ofLogWarning() << "Lua script error: " << message;
		///     });
		void setErrorCallback(std::function<void(std::string& message)> const &callback);
		
	/// \section Script Callbacks
	///
	/// these are default script callbacks which call a global function
	/// of the same name and arguments within the current Lua state
	///
	/// they fail silently if the function does not exist

		/// call Lua script setup() function
		void scriptSetup();

		/// call Lua script update() function
		void scriptUpdate();

		/// call Lua script draw() function
		void scriptDraw();

		/// call Lua script exit() function
		void scriptExit();

		/// call Lua script windowResized() function
		void scriptWindowResized(int w, int h);

		/// call Lua script keyPressed() function
		void scriptKeyPressed(int key);

		/// call Lua script keyReleased() function
		void scriptKeyReleased(int key);

		/// call Lua script mosueMoved() function
		void scriptMouseMoved(int x, int y );

		/// call Lua script mosueDragged() function
		void scriptMouseDragged(int x, int y, int button);

		/// call Lua script mousePressed() function
		void scriptMousePressed(int x, int y, int button);

		/// call Lua script mouseReleased() function
		void scriptMouseReleased(int x, int y, int button);

		/// call Lua script mouseScrolled() function
		void scriptMouseScrolled(int x, int y, float scrollX, float scrollY);

		/// call Lua script mouseEntered() function
		void scriptMouseEntered(int x, int y);

		/// call Lua script mouseExited() function
		void scriptMouseExited(int x, int y);

		/// call Lua script dragEvent() function
		void scriptDragEvent(ofDragInfo dragInfo);

		/// call Lua script gotMessage() function,
		/// note: sends msg contents as a std::string
		void scriptGotMessage(ofMessage msg);

		/// call Lua script touchDown() function
		void scriptTouchDown(ofTouchEventArgs &touch);

		/// call Lua script touchMoved() function
		void scriptTouchMoved(ofTouchEventArgs &touch);

		/// call Lua script touchUp() function
		void scriptTouchUp(ofTouchEventArgs &touch);

		/// call Lua script touchDoubleTap() function
		void scriptTouchDoubleTap(ofTouchEventArgs &touch);

		/// call Lua script touchCancelled() function
		void scriptTouchCancelled(ofTouchEventArgs &touch);
		
	/// \section Variables
	///
	/// check if a variable exists as a certain type in the Lua state,
	/// note: pushTable() must have been called when using the table index

		/// returns true if variable exists by name and is a bool
		bool isBool(const std::string& name);

		/// returns true if variable exists by index and is a bool,
		/// must be within a table
		/// note: integer indices start with 1!
		bool isBool(const unsigned int index);

		/// returns true if variable exists by name and is a number
		bool isNumber(const std::string& name);

		/// returns true if variable exists by index and is a number,
		/// must be within a table
		/// note: integer indices start with 1!
		bool isNumber(const unsigned int index);

		/// returns true if variable exists by name and is a string
		bool isString(const std::string& name);

		/// returns true if variable exists by index and is a string,
		/// must be within a table
		/// note: integer indices start with 1!
		bool isString(const unsigned int index);

		/// returns true if variable exists by name and is a function
		bool isFunction(const std::string& name);

		/// returns true if variable exists by index and is a function,
		/// must be within a table
		/// note: integer indices start with 1!
		bool isFunction(const unsigned int index);

		/// returns true if variable exists by name and is a table
		bool isTable(const std::string& name);

		/// returns true if variable exists by index and is a table,
		/// must be within a table
		/// note: integer indices start with 1!
		bool isTable(const unsigned int index);

		/// returns true if a variable *does not* exist by name,
		/// nil is the Lua equivalent of NULL
		bool isNil(const std::string& name);

		/// returns true if a variable *does not* exist by index,
		/// must be within a table, nil is the Lua equivalent of NULL
		/// note: integer indices start with 1!
		bool isNil(const unsigned int index);
	
	/// \section Table Operations
	
		/// create a new table with a given name
		void newTable(const std::string& tableName);

		/// create a new table at a given index, must be within a table
		/// note: integer indices start with 1!
		void newTable(const unsigned int& tableIndex);
	
		/// push table one level by name
		bool pushTable(const std::string& tableName);

		/// push table one level by index, must be within a table
		/// note: integer indices start with 1!
		bool pushTable(const unsigned int& tableIndex);

		/// pop table one level
		void popTable();

		/// pop all table levels
		void popAllTables();
		
		/// get the size of the current table,
		/// undefined if the table is not a sequence aka has a nil value somewhere
		unsigned int tableSize();

		/// get the size of a table with a given name,
		/// undefined if the table is not a sequence aka has a nil value somewhere
		unsigned int tableSize(const std::string& tableName);

		/// get the size of a table at a given index, must be within a table,
		/// undefined if the table is not a sequence aka has a nil value somewhere
		/// note: integer indices start with 1!
		unsigned int tableSize(const unsigned int& tableIndex);
		
		/// print the current table
		void printTable();

		/// print a table with a given name
		void printTable(const std::string& tableName);

		/// print a table at a given index, must be within a table
		/// note: integer indices start with 1!
		void printTable(const unsigned int& tableIndex);
		
		/// clear current table, removes all objects in the table
		void clearTable();

		/// clear table with a given name, removes all objects in the table
		void clearTable(const std::string& tableName);

		/// clear tabel at a given index, must be within a table,
		/// removes all objects in the table
		/// note: integer indices start with 1!
		void clearTable(const unsigned int& tableIndex);
	
	/// \section Reading
		
		/// get bool value by name,
		/// returns value or defaultValue if not found
		bool getBool(const std::string& name, bool defaultValue=false);

		/// get bool value by index, must be within a table
		/// returns value or defaultValue if not found
		/// note: integer indices start with 1!
		bool getBool(const unsigned int index, bool defaultValue=false);

		/// get number value by name
		/// returns value or defaultValue if not found
		lua_Number getNumber(const std::string& name, lua_Number devaultValue=0);

		/// get number value by index, must be within a table
		/// returns value or defaultValue if not found
		/// note: integer indices start with 1!
		lua_Number getNumber(const unsigned int index, lua_Number devaultValue=0);

		/// get string value by name
		/// returns value or defaultValue if not found
		std::string getString(const std::string& name, const std::string& defaultValue="");

		/// get string value by index, must be within a table
		/// returns value or defaultValue if not found
		/// note: integer indices start with 1!
		std::string getString(const unsigned int index, const std::string& defaultValue="");
		
		/// get a vector of bool values by table name,
		/// clears and copies values into v on success
		void getBoolVector(const std::string& tableName, std::vector<bool>& v);

		/// get a vector of bool values by table index, must be within a table
		/// clears and copies values into v on success
		/// note: integer indices start with 1!
		void getBoolVector(const unsigned int tableIndex, std::vector<bool>& v);

		/// get a vector of number values by table name,
		/// clears and copies values into v on success
		void getNumberVector(const std::string& tableName, std::vector<lua_Number>& v);

		/// get a vector of number values by table name, must be within a table
		/// clears and copies values into v on success
		/// note: integer indices start with 1!
		void getNumberVector(const unsigned int tableIndex, std::vector<lua_Number>& v);

		/// get a vector of string values by table name,
		/// clears and copies values into v on success
		void getStringVector(const std::string& tableName, std::vector<std::string>& v);

		/// get a vector of string values by table name, must be within a table
		/// clears and copies values into v on success
		/// note: integer indices start with 1!
		void getStringVector(const unsigned int tableIndex, std::vector<std::string>& v);
	
	/// \section Writing
		
		/// set bool value by name, creates if not existing
		void setBool(const std::string& name, bool value);

		/// set bool value by index, creates if not existing
		/// must be within a table
		/// note: integer indices start with 1!
		void setBool(const unsigned int index, bool value);

		/// set number value by name, creates if not existing
		void setNumber(const std::string& name, lua_Number value);

		/// set number value by index, creates if not existing
		/// must be within a table
		/// note: integer indices start with 1!
		void setNumber(const unsigned int index, lua_Number value);

		/// set string value by name, creates if not existing
		void setString(const std::string& name, const std::string value);

		/// set string value by index, creates if not existing
		/// must be within a table
		/// note: integer indices start with 1!
		void setString(const unsigned int index, const std::string value);

		/// set bool values by table name, table name must exist
		void setBoolVector(const std::string& tableName, std::vector<bool>& v);

		/// set bool values by table index, table index must exist
		/// must be within a table
		/// note: integer indices start with 1!
		void setBoolVector(const unsigned int tableIndex, std::vector<bool>& v);

		/// set number values by table name, table name must exist
		void setNumberVector(const std::string& tableName, std::vector<lua_Number>& v);

		/// set number values by table index, table index must exist
		/// must be within a table
		/// note: integer indices start with 1!
		void setNumberVector(const unsigned int tableIndex, std::vector<lua_Number>& v);

		/// set string values by table name, table name must exist
		void setStringVector(const std::string& tableName, std::vector<std::string>& v);

		/// set string values by table index, table index must exist
		/// must be within a table
		/// note: integer indices start with 1!
		void setStringVector(const unsigned int tableIndex, std::vector<std::string>& v);
	
		/// set a variable or table name to nil,
		/// essentially deletes it from the state
		void setNil(const std::string& name);

		/// set a variable or table index to nil, must be within a table
		/// essentially deletes it from the state
		/// note: integer indices start with 1!
		void setNil(const unsigned int index);
	
	/// \section Writing to a File
	///
	/// write simple variables and tables to a text file,
	/// this does not save actual scripts or the current state, only data in
	/// a given table
	
		/// write the current table to a given file writer,
		/// set recursive to false to only write data at the current level
		///
		/// note: cannot save the global table, you must call pushTable if you
	    /// don't save using a tableName
		void writeTable(ofxLuaFileWriter& writer, bool recursive=true);

		/// write named table to a given file writer,
		/// set recursive to false to only write data at the current level
		void writeTable(const std::string& tableName, ofxLuaFileWriter& writer, bool recursive=true);

		/// write the current table to a file,
		/// set recursive to false to only write data at the current level
		///
		/// note: cannot save the global table, you must call pushTable if you
	    /// don't save using a tableName
		bool writeTableToFile(const std::string& filename, bool recursive=true);

		/// write named table to a file,
		/// set recursive to false to only write data at the current level
		bool writeTableToFile(const std::string& tableName, const std::string& filename, bool recursive=true);
	
	/// \section Util

		/// send a Lua error message to any listeners and
		/// clear lua state if abortOnError is set
		virtual void errorOccurred(std::string& msg);
	
		/// get the current error message, an alternative to ofxLuaListener
		std::string getErrorMessage();
	
		/// print current stack length and contents from left to right,
		/// useful for debugging
		void printStack();
	
		/// get the raw Lua state, useful for custom Lua api code
		///
		/// example, call "myFunction(x, y)" in the Lua state:
		///
		///     int x = 20, y = 10;
		///     ofxLua lua;
		///     lua.init();
		///     
		///     lua_getglobal(lua, "myFunction");
		///     lua_pushinteger(lua, x);
		///     lua_pushinteger(lua, y);
		///     if(lua_pcall(lua, 2, 0, 0) != 0) {
		///         cout << "error running myFunction" << endl;
		///     }
		///
		///	note: make sure to call lua.init() before using the Lua state!
		///
		operator lua_State*() const {return L;}
	
		/// push a custom wrapped SWIG object pointer onto the stack,
		/// similar to lua_pushinteger, lua_pushstring, etc
		///
		/// typeName: name of the wrapped *orginal* C/C++ type as a std::string
		/// object: the pointer
		/// manageMemory: set to true if you want the Lua garbage collector to
		///               manage the memory pointed to
		///
		/// returns true if the pointer type was found and pushed
		///
		/// example, call a global Lua function "touchDown" with a touch event
		///
		///     ofTouchEventArgs *t = new ofTouchEventArgs(touch);
		///     lua_getglobal(lua, "touchDown");
		///     lua.pushobject("ofTouchArgEvents", t, true); // true: let Lua delete it
		///     lua_pcall(lua, 1, 0, 0);
		///
		bool pushobject(const std::string &typeName, void *object, bool manageMemory=true);
	
    protected:
		
		/// Lua stack top index
		static const int LUA_STACK_TOP = -1;
		
		/// returns true if an object exists by name
		bool exists(const std::string& name, int type);

		/// returns true if an object exists by index
		bool exists(const unsigned int index, int type);
		
		/// returns true is an object is of a certain type
		bool checkType(int stackIndex, int type);
	
		/// try to get value of a given type off of the top of the stack
		template <class T> T totype(int stackIndex, int type, T defaultValue);
	
		/// read a templated value from the state by name
		template <class T> T read(const std::string& name, int type, T defaultVal);

		/// read a templated value from the state by index
		template <class T> T read(const unsigned int index, int type, T defaultVal);

		/// read a table into a templated vector by name
		template <class T> void readVector(const std::string& name, std::vector<T>& v, int type, T defaultValue);

		/// read a table into a templated vector by index
		template <class T> void readVector(const unsigned int index, std::vector<T>& v, int type, T defaultValue);

		/// read the current table into a templated vector
		template <class T> void readVectorHelper(std::vector<T>& v, int type, T defaultValue);
	
		/// set a value of a given type by name
		template <class T> void settype(const std::string& name, int type, T value);

		/// set a value of a given type by index
		template <class T> void settype(unsigned int index, int type, T value);
	
		/// write a value to the state by name
		template <class T> void write(const std::string& name, int type, T value);

		/// write a value to the state by index
		template <class T> void write(const unsigned int index, int type, T value);
		
		/// write a vector into a table name
		template <class T> void writeVector(const std::string& name, int type, std::vector<T>& v);

		/// write a vector into a table index
		template <class T> void writeVector(const unsigned int index, int type, std::vector<T>& v);

		/// read a templated vector to the current table
		template <class T> void writeVectorHelper(int type, std::vector<T>& v);
	
		/// print current table
		void printTable(int stackIndex, int numTabs);
		
		/// writer current table to a buffer (values only)
		void writeTable(int stackIndex, ofxLuaFileWriter& writer, bool recursive);
	
		/// called when Lua state panics (hard crash)
		static int atPanic(lua_State *L);
	
		lua_State* L = NULL;       ///< the Lua state object
		bool abortOnError = false; ///< close the Lua state on error?

		/// nested table stack index, via name or index
		struct TableIndex {
			int type;           ///< LUA_TSTRING or LUA_TNUMBER
			std::string name;   ///< name index
			unsigned int index; ///< number index
			operator std::string() {
				if(type == LUA_TNUMBER) {
					return std::to_string(index);
				}
				return name;
			}
		};
		std::vector<TableIndex> tables; ///< the currently open table stack

		/// error event callback function pointer
		std::function<void(std::string& message)> errorCallback = nullptr;

		ofEvent<std::string> errorEvent; ///< error event object, std::string is error msg
		std::string errorMessage = "";   ///< current error message
};

// TEMPLATE FUNCTIONS

// TYPES

template <class T>
T ofxLua::totype(int stackIndex, int type, T defaultValue) { return defaultValue; }

template <class T>
void ofxLua::settype(const std::string& name, int type, T value) {}

template <class T>
void ofxLua::settype(unsigned int index, int type, T value) {}

// READ

template <class T>
T ofxLua::read(const std::string& name, int type, T defaultValue) {
	if(!isValid()) {
		return defaultValue;
	}
	
	// global variable?
	T ret = defaultValue;
	if(tables.size() == 0) {
		lua_getglobal(L, name.c_str());
		if(lua_type(L, LUA_STACK_TOP) == type) {
			ret = totype(LUA_STACK_TOP, type, defaultValue);
		}
		else {
			ofLogWarning("ofxLua") << "Couldn't read global var: \"" << name << "\"";
		}
		lua_pop(L, 1);
	}
	
	// in a table namespace
	else {
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't read var: \"" << name << "\""
				<< ", top of stack is not a table";
			return ret;
		}
		lua_getfield(L, LUA_STACK_TOP, name.c_str());
		if(lua_type(L, LUA_STACK_TOP) == type) {
			ret = totype(LUA_STACK_TOP, type, defaultValue);
		}
		else {
			ofLogWarning("ofxLua") << "Couldn't read table var: \"" << name << "\"";
		}
		lua_pop(L, 1);
	}
	
	return ret;
}

template <class T>
T ofxLua::read(const unsigned int index, int type, T defaultValue) {
	if(!isValid()) {
		return defaultValue;
	}
	
	// global variable?
	T ret = defaultValue;
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't read var by index, no open tables";
		return defaultValue;
	}
	
	// in a table namespace
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't read var " << index
			<< ", top of stack is not a table";
		return defaultValue;
	}
	lua_pushinteger(L, index);
	lua_gettable(L, LUA_STACK_TOP-1);
	if(lua_type(L, LUA_STACK_TOP) == type) {
		ret = totype(LUA_STACK_TOP, type, defaultValue);
	}
	else {
		ofLogWarning("ofxLua") << "Couldn't read table var " << index;
	}
	lua_pop(L, 1);
	
	return ret;
}

template <class T>
void ofxLua::readVector(const std::string& name, std::vector<T>& v, int type, T defaultValue) {
	if(!pushTable(name)) {
		return;
	}
	readVectorHelper(v, type, defaultValue);
	popTable();
}

template <class T>
void ofxLua::readVector(const unsigned int index, std::vector<T>& v, int type, T defaultValue) {
	if(!pushTable(index)) {
		return;
	}
	readVectorHelper(v, type, defaultValue);
	popTable();
}

template <class T>
void ofxLua::readVectorHelper(std::vector<T>& v, int type, T defaultValue) {
	if(!isValid()) {
		return;
	}
	
	v.clear();
	
	if(!lua_istable(L, LUA_STACK_TOP)) {
		std::string tname = "unknown";
		if(!tables.empty()) {
			tname = tables.back();
		}
		ofLogWarning("ofxLua") << "Couldn't read table \"" << tname << "\", stack var is not a table";
		ofLogWarning("ofxLua") << "Did you forget to call pushTable()?";
	}
	
	// push temp key & value onto the stack since lua_next will overwrite them
	lua_pushvalue(L, LUA_STACK_TOP); // stack: -1 => table
	lua_pushnil(L); // stack : -2 => table; -1 => nil;
	
	// iterate through elements, pushes key and value onto stack
	// stack: -3 => table; -2 => key; -1 => value
	while(lua_next(L, -2)) {
		if(lua_type(L, -1) == type) {
			v.push_back(totype(-1, type, defaultValue));
		}
		else {
			std::string tname = "unknown";
			if(!tables.empty()) {
				tname = tables.back();
			}
			ofLogWarning("ofxLua") << "Couldn't convert type when reading table \"" << tname << "\"";
		}
		lua_pop(L, 1); // stack: -2 => table; -1 => key
	}
	
	// stack: -1 => table
	lua_pop(L, 1); // stack:
}

// WRITE

template <class T>
void ofxLua::write(const std::string& name, int type, T value) {
	if(!isValid()) {
		return;
	}
	
	// global variable?
	if(tables.empty()) {
		lua_pushglobaltable(L);
		settype<T>(name, type, value);
		lua_pop(L, 1);
	}
	
	// in a table namespace
	else {
		if(!lua_istable(L, LUA_STACK_TOP)) {
			ofLogWarning("ofxLua") << "Couldn't write var: \"" << name << "\""
				<< ", top of stack is not a table";
			return;
		}
		settype<T>(name, type, value);
	}
}

template <class T>
void ofxLua::write(const unsigned int index, int type, T value) {
	
	// global variable?
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write global var by index, no open tables";
		return;
	}
	
	// in a table namespace
	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't write var at index " << index
			<< ", top of stack is not a table";
		return;
	}
	
	settype<T>(index, type, value);
}

template <class T>
void ofxLua::writeVector(const std::string& name, int type, std::vector<T>& v) {
	if(!pushTable(name)) {
		return;
	}
	writeVectorHelper(type, v);
	popTable();
}

template <class T>
void ofxLua::writeVector(const unsigned int index, int type, std::vector<T>& v) {
	if(!pushTable(index)) {
		return;
	}
	writeVectorHelper(type, v);
	popTable();
}

template <class T>
void ofxLua::writeVectorHelper(int type, std::vector<T>& v) {

	// global variable?
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write table vector, no open tables";
		return;
	}
	std::string tname = tables.back();
	
	if(v.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write table \"" << tname << "\", vector is empty";
		return;
	}

	if(!lua_istable(L, LUA_STACK_TOP)) {
		ofLogWarning("ofxLua") << "Couldn't write table \"" << tname << "\", stack var is not a table";
	}
	
	// remove all elements
	clearTable();
	
	// add new variables
	for(size_t i = 0; i < v.size(); ++i) {
		settype<T>(i+1, type, v[i]);
	}
}
