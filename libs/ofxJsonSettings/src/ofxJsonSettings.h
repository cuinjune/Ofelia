//
//  ofxJsonSettings.h
//  ofxJsonSettings
//
//  Created by Matt Felsen on 12/7/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

// helper for shorter syntax
class ofxJsonSettings;
typedef ofxJsonSettings Settings;

class ofxJsonSettings {
public:
	ofxJsonSettings();

	// Singleton..
	static ofxJsonSettings& get() {
		static ofxJsonSettings instance;
		return instance;
	}

	void setDelimiter(string delim);
	bool load(string file = "settings.json");
	bool save(string file = "settings.json", bool prettyPrint = true);

	string getAsJsonString(); //in case you want to print / read

	// Use this to get values
	// These return references, so you can bind directly to a GUI library that
	// supports variable binding
	static string& getString(string key);
	static bool& getBool(string key);
	static int& getInt(string key);
	static float& getFloat(string key);
	static double& getDouble(string key);
	static ofVec2f& getVec2(string key);
	static ofVec3f& getVec3(string key);
	static ofVec4f& getVec4(string key);
	static ofColor& getColor(string key);
	static ofxJSON getJson(string key);

	// Use this to check if a key exists
	static bool exists(string key);

	static bool remove(string key);

	ofEvent<void> settingsLoaded;
	ofEvent<void> settingsSaved;

protected:
	string& _stringVal(string& key);
	bool& _boolVal(string& key);
	int& _intVal(string& key);
	float& _floatVal(string& key);
	double& _doubleVal(string& key);
	ofVec2f& _vec2Val(string& key);
	ofVec3f& _vec3Val(string& key);
	ofVec4f& _vec4Val(string& key);
	ofColor& _colorVal(string& key);
	ofxJSON _jsonVal(string& key);

	string _stringValFromJson(ofxJSON& data, string& key);
	bool _boolValFromJson(ofxJSON& data, string& key);
	int _intValFromJson(ofxJSON& data, string& key);
	float _floatValFromJson(ofxJSON& data, string& key);
	double _doubleValFromJson(ofxJSON& data, string& key);
	ofVec2f _vec2ValFromJson(ofxJSON& data, string& key);
	ofVec3f _vec3ValFromJson(ofxJSON& data, string& key);
	ofVec4f _vec4ValFromJson(ofxJSON& data, string& key);
	ofColor _colorValFromJson(ofxJSON& data, string& key);

	bool _exists(string key);
	bool _remove(string key);

	ofxJSON getNestedChild(ofxJSON data, string key, bool supressWarning = false);
	ofxJSON getNestedChild(ofxJSON data, vector<string> keys, bool supressWarning = false);

	template<typename T>
	void setNestedChild(ofxJSON& data, string key, T val);

	template<typename T>
	void setNestedChild(ofxJSON& data, vector<string> keys, T val);

	template<typename T>
	void cacheToJson(T& container, ofxJSON& data);

	void cacheToJson(unordered_map<string,ofVec2f>& container, ofxJSON& data);
	void cacheToJson(unordered_map<string,ofVec3f>& container, ofxJSON& data);
	void cacheToJson(unordered_map<string,ofVec4f>& container, ofxJSON& data);
	void cacheToJson(unordered_map<string,ofColor>& container, ofxJSON& data);

	template<typename T>
	bool exists(T& container, const string &key);

	template<typename T>
	void printMap(T& container, const string& text = "");

	ofxJSON jsonStore;
	unordered_map<string,string> stringMap;
	unordered_map<string,int> intMap;
	unordered_map<string,bool> boolMap;
	unordered_map<string,float> floatMap;
	unordered_map<string,double> doubleMap;
	unordered_map<string,ofVec2f> vec2Map;
	unordered_map<string,ofVec3f> vec3Map;
	unordered_map<string,ofVec4f> vec4Map;
	unordered_map<string,ofColor> colorMap;

	string delimiter;

private:
	// make sure there are no copies
	ofxJsonSettings(ofxJsonSettings const&);
	void operator=(ofxJsonSettings const&);
};
