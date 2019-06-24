/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 * This project uses libpd, copyrighted by Miller Puckette and others using the
 * "Standard Improved BSD License". See the file "LICENSE.txt" in src/pd.
 *
 * See http://gitorious.org/pdlib/pages/Libpd for documentation
 *
 */
#include "ofxPd.h"

#include <algorithm>
#include "ofUtils.h"
#include "ofLog.h"

// needed for libpd audio passing
#ifndef USEAPI_DUMMY
	#define USEAPI_DUMMY
#endif

using namespace std;
using namespace pd;

//--------------------------------------------------------------------
ofxPd::ofxPd() : PdBase() {
	inBuffer = NULL;
	computing = false;
	clear();
}

//--------------------------------------------------------------------
ofxPd::~ofxPd() {
    clear();
}

//--------------------------------------------------------------------
bool ofxPd::init(const int numOutChannels, const int numInChannels, 
                 const int sampleRate, const int ticksPerBuffer, bool queued) {
	
	// init pd
	if(!PdBase::init(numInChannels, numOutChannels, sampleRate, queued)) {
		ofLogError("Pd") << "could not init";
		clear();
		return false;
	}
	
	ticks = ticksPerBuffer;
	bsize = ticksPerBuffer*blockSize();
	srate = sampleRate;
	inChannels = numInChannels;
	outChannels = numOutChannels;

	// allocate buffers
	inBuffer = new float[numInChannels*bsize];

	ofLogVerbose("Pd") <<"inited";
	ofLogVerbose("Pd") <<" samplerate: " << sampleRate;
	ofLogVerbose("Pd") <<" channels in: " << numInChannels;
	ofLogVerbose("Pd") <<" channels out: " << numOutChannels;
	ofLogVerbose("Pd") <<" ticks: " << ticksPerBuffer;
	ofLogVerbose("Pd") <<" block size: " << blockSize();
	ofLogVerbose("Pd") <<" calc buffer size: " << bsize;
	ofLogVerbose("Pd") <<" queued: " << (queued ? "yes" : "no");

	return true;
}

void ofxPd::clear() {
	
	// this seems hacky, but (so far) gets rid of a deadlock on Windows
	// which causes a hang on exit
	//
	// hopefully to be fixed for real at a future point ...
//	#ifndef TARGET_WIN32
//		lock();
//	#endif
	if(inBuffer != NULL) {
		delete[] inBuffer;
		inBuffer = NULL;
	}
	PdContext::instance().clear();
//	#ifndef TARGET_WIN32
//		unlock();
//	#endif
	unsubscribeAll();

	channels.clear();

	// add default global channel
	Channel c;
	channels.insert(make_pair(-1, c));
	
	ticks = 0;
	bsize = 0;
	srate = 0;
	inChannels = 0;
	outChannels = 0;
}

//--------------------------------------------------------------------
void ofxPd::addToSearchPath(const std::string& path) {
	string fullpath = ofFilePath::getAbsolutePath(ofToDataPath(path));
	ofLogVerbose("Pd") << "adding search path: "+fullpath;
	PdBase::addToSearchPath(fullpath.c_str());
}

void ofxPd::clearSearchPath() {
	ofLogVerbose("Pd") << "clearing search paths";
	PdBase::clearSearchPath();
}

//--------------------------------------------------------------------
Patch ofxPd::openPatch(const std::string& patch) {

	string fullpath = ofFilePath::getAbsolutePath(ofToDataPath(patch));
	string file = ofFilePath::getFileName(fullpath);
	string folder = ofFilePath::getEnclosingDirectory(fullpath);
	
	// trim the trailing slash Poco::Path always adds ... blarg
	if(folder.size() > 0 && folder.at(folder.size()-1) == '/') {
		folder.erase(folder.end()-1);
	}
	
	ofLogVerbose("Pd") << "opening patch: "+file+" path: "+folder;

	// [; pd open file folder(
	Patch p = PdBase::openPatch(file.c_str(), folder.c_str());
 	if(!p.isValid()) {
		ofLogError("Pd") << "opening patch \""+file+"\" failed";
	}
	
	return p;
}

pd::Patch ofxPd::openPatch(pd::Patch& patch) {
	ofLogVerbose("Pd") << "opening patch: "+patch.filename()+" path: "+patch.path();
	Patch p = PdBase::openPatch(patch);
	if(!p.isValid()) {
		ofLogError("Pd") << "opening patch \""+patch.filename()+"\" failed";
	}
	return p;
}

void ofxPd::closePatch(const std::string& patch) {
	ofLogVerbose("Pd") << "closing path: "+patch;
	PdBase::closePatch(patch);
}

void ofxPd::closePatch(Patch& patch) {
	ofLogVerbose("Pd") << "closing patch: "+patch.filename();
	PdBase::closePatch(patch);
}	

//--------------------------------------------------------------------
void ofxPd::computeAudio(bool state) {
	if(state) {
		ofLogVerbose("Pd") << "audio processing on";
	}
	else {
		ofLogVerbose("Pd") << "audio processing off";
	}
	computing = state;

	// [; pd dsp $1(
	PdBase::computeAudio(state);
}
void ofxPd::start() {
	// [; pd dsp 1(
	computeAudio(true);
}

void ofxPd::stop() {	
	// [; pd dsp 0(
	computeAudio(false);
}

//----------------------------------------------------------
void ofxPd::subscribe(const std::string& source) {
	if(exists(source)) {
		ofLogWarning("Pd") << "subscribe: ignoring duplicate source";
		return;
	}
	PdBase::subscribe(source);
	Source s;
	sources.insert(pair<string,Source>(source, s));
}

void ofxPd::unsubscribe(const std::string& source) {
	map<string,Source>::iterator iter;
	iter = sources.find(source);
	if(iter == sources.end()) {
		ofLogWarning("Pd") << "unsubscribe: ignoring unknown source";
		return;
	}
	PdBase::unsubscribe(source);
	sources.erase(iter);
}

bool ofxPd::exists(const std::string& source) {
	if(sources.find(source) != sources.end()) {
		return true;
	}
	return false;
}

void ofxPd::unsubscribeAll(){

    PdBase::unsubscribeAll();
	sources.clear();

	// add default global source
	Source s;
	sources.insert(make_pair("", s));
}

//--------------------------------------------------------------------
void ofxPd::addReceiver(PdReceiver& receiver) {
	
	pair<set<PdReceiver*>::iterator, bool> ret;
	ret = receivers.insert(&receiver);
	if(!ret.second) {
		ofLogWarning("Pd") << "addReceiver: ignoring duplicate receiver";
		return;
	}

	// set PdBase receiver on adding first reciever
	if(receivers.size() == 1) {
		PdBase::setReceiver(this);
	}

	// receive from all sources by default
	receiveSource(receiver);
}

void ofxPd::removeReceiver(PdReceiver& receiver) {
	
	// exists?
	set<PdReceiver*>::iterator r_iter;
	r_iter = receivers.find(&receiver);
	if(r_iter == receivers.end()) {
		ofLogWarning("Pd") << "removeReceiver: ignoring unknown receiver";
		return;
	}
	receivers.erase(r_iter);
	
	// clear PdBase receiver on removing last reciever
	if(receivers.size() == 0) {
		PdBase::setReceiver(NULL);
	}

	// remove from all sources
	ignoreSource(receiver);
}

bool ofxPd::receiverExists(PdReceiver& receiver) {
	if(receivers.find(&receiver) != receivers.end())
		return true;
	return false;
}

void ofxPd::clearReceivers() {

	receivers.clear();
	
	map<string,Source>::iterator iter;
	for(iter = sources.begin(); iter != sources.end(); ++iter) {
		iter->second.receivers.clear();
	}
	
	PdBase::setReceiver(NULL);
}

//----------------------------------------------------------
int ofxPd::ticksPerBuffer() {
	return ticks;
}

int ofxPd::bufferSize() {
	return bsize;
}

int ofxPd::sampleRate() {
	return srate;
}

int ofxPd::numInChannels() {
	return inChannels;
}

int ofxPd::numOutChannels() {
	return outChannels;
}

bool ofxPd::isComputingAudio() {
	return computing;
}

//----------------------------------------------------------
void ofxPd::receiveSource(PdReceiver& receiver, const std::string& source) {

	if(!receiverExists(receiver)) {
		ofLogWarning("Pd") << "receive: unknown receiver, call addReceiver first";
		return;
	}
	
	if(!exists(source)) {
		ofLogWarning("Pd") << "receive: unknown source, call subscribe first";
		return;
	}
	
	// global source (all sources)
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	
	// subscribe to specific source
	if(source != "") {
	
		// make sure global source is ignored
		if(g_iter->second.receiverExists(&receiver)) {
			g_iter->second.removeReceiver(&receiver);
		}
		
		// receive from specific source
		map<string,Source>::iterator s_iter;
		s_iter = sources.find(source);
		s_iter->second.addReceiver(&receiver);
	}
	else {
		// make sure all sources are ignored
		ignoreSource(receiver);
	
		// receive from the global source
		g_iter->second.addReceiver(&receiver);
	}
}

void ofxPd::ignoreSource(PdReceiver& receiver, const std::string& source) {

	if(!receiverExists(receiver)) {
		ofLogWarning("Pd") << "ignore: ignoring unknown receiver";
		return;
	}

	if(!exists(source)) {
		ofLogWarning("Pd") << "ignore: ignoring unknown source";
		return;
	}
	
	map<string,Source>::iterator s_iter;
	
	// unsubscribe from specific source
	if(source != "") {
	
		// global source (all sources)
		map<string,Source>::iterator g_iter;
		g_iter = sources.find("");
	
		// negation from global
		if(g_iter->second.receiverExists(&receiver)) {
			
			// remove from global
			g_iter->second.removeReceiver(&receiver);
			
			// add to *all* other sources
			for(s_iter = sources.begin(); s_iter != sources.end(); ++s_iter) {
				if(s_iter != g_iter) {
					s_iter->second.addReceiver(&receiver);
				}
			}
		}
		
		// remove from source
		s_iter = sources.find(source);
		s_iter->second.removeReceiver(&receiver);
	}
	else {	// ignore all sources	
		for(s_iter = sources.begin(); s_iter != sources.end(); ++s_iter) {
			s_iter->second.removeReceiver(&receiver);
		}
	}
}

bool ofxPd::isReceivingSource(PdReceiver& receiver, const std::string& source) {
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(source);
	if(s_iter != sources.end() && s_iter->second.receiverExists(&receiver)) {
		return true;
	}
	return false;
}

//----------------------------------------------------------
void ofxPd::addMidiReceiver(PdMidiReceiver& receiver) {

	pair<set<PdMidiReceiver*>::iterator, bool> ret;
	ret = midiReceivers.insert(&receiver);
	if(!ret.second) {
		ofLogWarning("Pd") << "addMidiReceiver: ignoring duplicate receiver";
		return;
	}

	// set PdBase receiver on adding first reciever
	if(midiReceivers.size() == 1) {
		PdBase::setMidiReceiver(this);
	}

	// receive from all channels by default
	receiveMidiChannel(receiver);
}

void ofxPd::removeMidiReceiver(PdMidiReceiver& receiver) {

	// exists?
	set<PdMidiReceiver*>::iterator r_iter;
	r_iter = midiReceivers.find(&receiver);
	if(r_iter == midiReceivers.end()) {
		ofLogWarning("Pd") << "removeMidiReceiver: ignoring unknown receiver";
		return;
	}
	midiReceivers.erase(r_iter);
	
	// clear PdBase receiver on removing last reciever
	if(receivers.size() == 0) {
		PdBase::setMidiReceiver(NULL);
	}

	// remove from all sources
	ignoreMidiChannel(receiver);
}

bool ofxPd::midiReceiverExists(PdMidiReceiver& receiver) {
	if(midiReceivers.find(&receiver) != midiReceivers.end()) {
		return true;
	}
	return false;
}

void ofxPd::clearMidiReceivers() {

	midiReceivers.clear();
	
	map<int,Channel>::iterator iter;
	for(iter = channels.begin(); iter != channels.end(); ++iter) {
		iter->second.receivers.clear();
	}

	PdBase::setMidiReceiver(NULL);
}

//----------------------------------------------------------
void ofxPd::receiveMidiChannel(PdMidiReceiver& receiver, int channel) {

	if(!midiReceiverExists(receiver)) {
		ofLogWarning("Pd") << "receiveMidi: unknown receiver, call addMidiReceiver first";
		return;
	}

	// handle bad channel numbers
	if(channel < 0) {
		channel = 0;
	}

	// insert channel if it dosen't exist yet
	if(channels.find(channel) == channels.end()) {
		Channel c;
		channels.insert(pair<int,Channel>(channel, c));
	}

	// global channel (all channels)
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	
	// subscribe to specific channel
	if(channel != 0) {
	
		// make sure global channel is ignored
		if(g_iter->second.midiReceiverExists(&receiver)) {
			g_iter->second.removeMidiReceiver(&receiver);
		}
		
		// receive from specific channel
		map<int,Channel>::iterator c_iter;
		c_iter = channels.find(channel);
		c_iter->second.addMidiReceiver(&receiver);
	}
	else {
		// make sure all channels are ignored
		ignoreMidiChannel(receiver);

		// receive from the global channel
		g_iter->second.addMidiReceiver(&receiver);
	}
}

void ofxPd::ignoreMidiChannel(PdMidiReceiver& receiver, int channel) {

	if(!midiReceiverExists(receiver)) {
		ofLogWarning("Pd") << "ignoreMidi: ignoring unknown receiver";
		return;
	} 

	// handle bad channel numbers
	if(channel < 0) {
		channel = 0;
	}

	// insert channel if it dosen't exist yet
	if(channels.find(channel) == channels.end()) {
		Channel c;
		channels.insert(pair<int,Channel>(channel, c));
	}

	map<int,Channel>::iterator c_iter;
	
	// unsubscribe from specific channel
	if(channel != 0) {
	
		// global channel (all channels)
		map<int,Channel>::iterator g_iter;
		g_iter = channels.find(0);
	
		// negation from global
		if(g_iter->second.midiReceiverExists(&receiver)) {
			
			// remove from global
			g_iter->second.removeMidiReceiver(&receiver);
			
			// add to *all* other channels
			for(c_iter = channels.begin(); c_iter != channels.end(); ++c_iter) {
				if(c_iter != g_iter) {
					c_iter->second.addMidiReceiver(&receiver);
				}
			}
		}
		
		// remove from channel
		c_iter = channels.find(channel);
		c_iter->second.removeMidiReceiver(&receiver);
	}
	else {	// ignore all sources	
		for(c_iter = channels.begin(); c_iter != channels.end(); ++c_iter) {
			c_iter->second.removeMidiReceiver(&receiver);
		}
	}
}

bool ofxPd::isReceivingMidiChannel(PdMidiReceiver& receiver, int channel) {

	// handle bad channel numbers
	if(channel < 0) {
		channel = 0;
	}

	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end() && c_iter->second.midiReceiverExists(&receiver))
		return true;
	return false;
}

//----------------------------------------------------------
void ofxPd::sendNoteOn(const int channel, const int pitch, const int velocity) {
	PdBase::sendNoteOn(channel-1, pitch, velocity);
}

void ofxPd::sendControlChange(const int channel, const int control, const int value) {
	PdBase::sendControlChange(channel-1, control, value);
}

void ofxPd::sendProgramChange(const int channel, int program) {
	PdBase::sendProgramChange(channel-1, program-1);
}

void ofxPd::sendPitchBend(const int channel, const int value) {
	PdBase::sendPitchBend(channel-1, value);
}

void ofxPd::sendAftertouch(const int channel, const int value) {
	PdBase::sendAftertouch(channel-1, value);
}

void ofxPd::sendPolyAftertouch(const int channel, int pitch, int value) {
	PdBase::sendPolyAftertouch(channel-1, pitch, value);
}

//----------------------------------------------------------
void ofxPd::audioIn(float* input, int bufferSize, int nChannels) {
	try {
		if(inBuffer != NULL) {
			if(bufferSize != bsize || nChannels != inChannels) {
				ticks = bufferSize/blockSize();
				bsize = bufferSize;
				inChannels = nChannels;
				ofLogVerbose("Pd") << "buffer size or num input channels updated";
				init(outChannels, inChannels, srate, ticks, isQueued());
				PdBase::computeAudio(computing);
			}
			memcpy(inBuffer, input, bufferSize*nChannels*sizeof(float));
		}
	}
	catch (...) {
		ofLogError("Pd") << "could not copy input buffer";
	}
}

void ofxPd::audioOut(float* output, int bufferSize, int nChannels) {
	if(inBuffer != NULL) {
		if(bufferSize != bsize || nChannels != outChannels) {
			ticks = bufferSize/blockSize();
			bsize = bufferSize;
			outChannels = nChannels;
			ofLogVerbose("Pd") << "buffer size or num output channels updated";
			init(outChannels, inChannels, srate, ticks, isQueued());
			PdBase::computeAudio(computing);
		}
		if(!PdBase::processFloat(ticks, inBuffer, output)) {
			ofLogError("Pd") << "could not process output buffer";
		}
	}
}

/* ***** PROTECTED ***** */

//----------------------------------------------------------
void ofxPd::print(const std::string& message) {

	ofLogVerbose("Pd") << "print: " << message;

	// broadcast
	set<PdReceiver*>::iterator iter;
	for(iter = receivers.begin(); iter != receivers.end(); ++iter) {
		(*iter)->print(message);
	}
}

void ofxPd::receiveBang(const std::string& dest) {

	ofLogVerbose("Pd") << "bang: " << dest;
	
	set<PdReceiver*>::iterator r_iter;
	set<PdReceiver*>* r_set;

	// send to global receivers
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveBang(dest);
	}
	
	// send to subscribed receivers
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(dest);
	r_set = &s_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveBang(dest);
	}
}

void ofxPd::receiveFloat(const std::string& dest, float value) {

	ofLogVerbose("Pd") << "float: " << dest << " " << value;

	set<PdReceiver*>::iterator r_iter;
	set<PdReceiver*>* r_set;

	// send to global receivers
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveFloat(dest, value);
	}

	// send to subscribed receivers
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(dest);
	r_set = &s_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveFloat(dest, value);
	}
}

void ofxPd::receiveSymbol(const std::string& dest, const std::string& symbol) {

	ofLogVerbose("Pd") << "symbol: " << dest << " " << symbol;

	set<PdReceiver*>::iterator r_iter;
	set<PdReceiver*>* r_set;

	// send to global receivers
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveSymbol(dest, (string) symbol);
	}

	// send to subscribed receivers
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(dest);
	r_set = &s_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveSymbol(dest, (string) symbol);
	}
}

void ofxPd::receiveList(const std::string& dest, const List& list) {

	ofLogVerbose("Pd") << "list: " << dest << " " << list.toString();

	set<PdReceiver*>::iterator r_iter;
	set<PdReceiver*>* r_set;

	// send to global receivers
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveList(dest, list);
	}

	// send to subscribed receivers
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(dest);
	r_set = &s_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveList(dest, list);
	}
}

void ofxPd::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {

	ofLogVerbose("Pd") << "message: " << dest << " " << msg << " " << list.toString();

	set<PdReceiver*>::iterator r_iter;
	set<PdReceiver*>* r_set;

	// send to global receivers
	map<string,Source>::iterator g_iter;
	g_iter = sources.find("");
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveMessage(dest, msg, list);
	}

	// send to subscribed receivers
	map<string,Source>::iterator s_iter;
	s_iter = sources.find(dest);
	r_set = &s_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveMessage(dest, msg, list);
	}
}

//----------------------------------------------------------
void ofxPd::receiveNoteOn(const int channel, const int pitch, const int velocity) {

	ofLogVerbose("Pd") << "note on: " << channel+1 << " " << pitch << " " << velocity;

	set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;

	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveNoteOn(channel+1, pitch, velocity);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receiveNoteOn(channel+1, pitch, velocity);
		}
	}
}

void ofxPd::receiveControlChange(const int channel, const int controller, const int value) {

	ofLogVerbose("Pd") << "control change: " << channel+1 << " " << controller << " " << value;

	set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;
 
	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveControlChange(channel+1, controller, value);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receiveControlChange(channel+1, controller, value);
		}
	}
}

void ofxPd::receiveProgramChange(const int channel, const int value) {

	ofLogVerbose("Pd") << "program change: " << channel+1 << " " << value+1;

    set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;

	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveProgramChange(channel+1, value+1);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receiveProgramChange(channel+1, value+1);
		}
	}
}

void ofxPd::receivePitchBend(const int channel, const int value) {

	ofLogVerbose("Pd") << "pitch bend: " << channel+1 << value;

    set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;

	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receivePitchBend(channel+1, value);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receivePitchBend(channel+1, value);
		}
	}
}

void ofxPd::receiveAftertouch(const int channel, const int value) {

	ofLogVerbose("Pd") << "aftertouch: " << channel+1 << value;

	set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;

	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receiveAftertouch(channel+1, value);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receiveAftertouch(channel+1, value);
		}
	}
}

void ofxPd::receivePolyAftertouch(const int channel, const int pitch, const int value) {

	ofLogVerbose("Pd") << "poly aftertouch: " << channel+1 << " " << pitch << " " << value;

    set<PdMidiReceiver*>::iterator r_iter;
	set<PdMidiReceiver*>* r_set;

	// send to global receivers
	map<int,Channel>::iterator g_iter;
	g_iter = channels.find(0);
	r_set = &g_iter->second.receivers;
	for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
		(*r_iter)->receivePolyAftertouch(channel+1, pitch, value);
	}

	// send to subscribed receivers
	map<int,Channel>::iterator c_iter;
	c_iter = channels.find(channel);
	if(c_iter != channels.end()) {
		r_set = &c_iter->second.receivers;
		for(r_iter = r_set->begin(); r_iter != r_set->end(); ++r_iter) {
			(*r_iter)->receivePolyAftertouch(channel+1, pitch, value);
		}
	}
}

void ofxPd::receiveMidiByte(const int port, const int byte) {

	ofLogVerbose("Pd") << "midi byte: " << port << " " << byte;

	set<PdMidiReceiver*>& r_set = midiReceivers;
	set<PdMidiReceiver*>::iterator iter;
	for(iter = r_set.begin(); iter != r_set.end(); ++iter) {
		(*iter)->receiveMidiByte(port, byte);
	}
}
