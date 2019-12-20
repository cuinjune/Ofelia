/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxLua for documentation
 *
 */
#include "ofxLuaFileWriter.h"

#include "ofUtils.h"

//------------------------------------------------------------------------------
ofxLuaFileWriter::ofxLuaFileWriter() {
	commentBlock = false;
}

//------------------------------------------------------------------------------
ofxLuaFileWriter::~ofxLuaFileWriter() {}

//------------------------------------------------------------------------------
bool ofxLuaFileWriter::saveToFile(const std::string& filename) {
	ofBuffer b(buffer);
	return ofBufferToFile(ofToDataPath(filename), b);
}

//------------------------------------------------------------------------------
void ofxLuaFileWriter::clear() {
	buffer.clear();
	tables.clear();
}

//------------------------------------------------------------------------------
void ofxLuaFileWriter::newLine() {
	buffer << std::endl;
}

void ofxLuaFileWriter::writeComment(const std::string& comment) {
	buffer << "-- " << comment << std::endl;
}

void ofxLuaFileWriter::beginCommentBlock() {
	if(commentBlock) {
		ofLogWarning("ofxLua") << "Comment block already begun";
		return;
	}
	commentBlock = true;
	buffer << "--[[" << std::endl;
}

void ofxLuaFileWriter::endCommentBlock() {
	if(!commentBlock) {
		ofLogWarning("ofxLua") << "Comment block not begun";
		return;
	}
	commentBlock = false;
	buffer << "--]]" << std::endl;
}

void ofxLuaFileWriter::writeLine(const std::string& line) {
	buffer << line << std::endl;
}

void ofxLuaFileWriter::beginTable(const std::string& tableName) {
	if(tables.empty()) {
		buffer << tableName << " = {}" << std::endl;
	}
	else {
		writeTablePath();
		buffer << "." << tableName << " = {}" << std::endl;
	}
	tables.push_back({LUA_TSTRING, tableName, 0});
}

void ofxLuaFileWriter::beginTable(const unsigned int tableIndex) {
	if(tables.empty()) {
		buffer << tableIndex << " = {}" << std::endl;
	}
	else {
		writeTablePath();
		buffer << "[" << tableIndex << "] = {}" << std::endl;
	}
	tables.push_back({LUA_TNUMBER, "", (unsigned int)tableIndex});
}

void ofxLuaFileWriter::endTable() {
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "No tables to end, call beginTable";
	}
	else {
		tables.pop_back();
	}
}

//------------------------------------------------------------------------------
void ofxLuaFileWriter::writeBool(const std::string& name, bool value) {
	write<bool>(name, LUA_TBOOLEAN, value);
}

void ofxLuaFileWriter::writeBool(const unsigned int index, bool value) {
	write<bool>(index, LUA_TBOOLEAN, value);
}

void ofxLuaFileWriter::writeNumber(const std::string& name, lua_Number value) {
	write<lua_Number>(name, LUA_TNUMBER, value);
}

void ofxLuaFileWriter::writeNumber(const unsigned int index, lua_Number value) {
	write<lua_Number>(index, LUA_TNUMBER, value);
}

void ofxLuaFileWriter::writeString(const std::string& name, std::string value) {
	write<std::string>(name, LUA_TSTRING, value);
}

void ofxLuaFileWriter::writeString(const unsigned int index, std::string value) {
	write<std::string>(index, LUA_TSTRING, value);
}

void ofxLuaFileWriter::writeBoolVector(const std::string& tableName, std::vector<bool>& v) {
	writeVector<bool>(tableName, LUA_TBOOLEAN, v);
}

void ofxLuaFileWriter::writeBoolVector(const unsigned int index, std::vector<bool>& v) {
	writeVector<bool>(index, LUA_TBOOLEAN, v);
}

void ofxLuaFileWriter::writeNumberVector(const std::string& tableName, std::vector<lua_Number>& v) {
	writeVector<lua_Number>(tableName, LUA_TNUMBER, v);
}

void ofxLuaFileWriter::writeNumberVector(const unsigned int index, std::vector<lua_Number>& v) {
	writeVector<lua_Number>(index, LUA_TNUMBER, v);
}

void ofxLuaFileWriter::writeStringVector(const std::string& tableName, std::vector<std::string>& v) {
	writeVector<std::string>(tableName, LUA_TSTRING, v);
}

void ofxLuaFileWriter::writeStringVector(const unsigned int index, std::vector<std::string>& v) {
	writeVector<std::string>(index, LUA_TSTRING, v);
}

// PRIVATE

//------------------------------------------------------------------------------
template <> void ofxLuaFileWriter::writetype<bool>(int type, bool value) {
	buffer << (value ? "true" : "false");
}

// catch vector<bool> internal type since it isn't actually a bool
template <> void ofxLuaFileWriter::writetype<std::vector<bool>::reference>(int type, std::vector<bool>::reference value) {
	buffer << ((bool)value ? "true" : "false");
}

template <> void ofxLuaFileWriter::writetype<lua_Number>(int type, lua_Number value) {
	buffer << value;
}

template <> void ofxLuaFileWriter::writetype<std::string>(int type, std::string value) {
	buffer << "\"" << value << "\"";
}

//------------------------------------------------------------------------------
// writes path for all open tables ie "t1.t2[3]"
void ofxLuaFileWriter::writeTablePath() {
	if(tables.empty()) {
		return;
	}
	buffer << (std::string)tables[0];
	for(size_t i = 1; i < tables.size(); ++i) {
		if(tables[i].type == LUA_TSTRING) {
			buffer << "." << (std::string)tables[i];
		}
		else {
			buffer << "[" << (std::string)tables[i] << "]";
		}
	}
}
