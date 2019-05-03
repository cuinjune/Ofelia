// sound folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofSoundBaseTypes.h -----

// handled in main.i

// ----- ofFmodSoundPlayer.h -----

// not needed

// ----- ofOpenALSoundPlayer.h -----

// not needed

// ----- ofRtAudioSoundStream.h -----

// not needed

// ----- ofSoundBuffer.h -----

// not needed

// ----- ofSoundStream.h -----

// ignore overloaded functions
%ignore ofSoundStream::setInput(ofBaseSoundInput &soundInput);
%ignore ofSoundStream::setOutput(ofBaseSoundOutput &soundOutput);

%include "sound/ofSoundStream.h"

// ----- ofSoundPlayer.h -----

%ignore ofBaseSoundPlayer;
class ofBaseSoundPlayer {};

// DIFF: ofSoundPlayer.h: ignoring global FMOD functions
%ignore ofSoundStopAll;
%ignore ofSoundShutdown;
%ignore ofSoundSetVolume;
%ignore ofSoundUpdate;
%ignore ofSoundGetSpectrum;

%include "sound/ofSoundPlayer.h"

// ----- ofSoundUtils.h -----

// not needed
