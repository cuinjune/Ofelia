// video folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofVideoBaseTypes.h -----

// handled in main.i

// ----- ofVideoGrabber.h -----

// DIFF: ofVideoGrabber.h: ignore getGrabber/setGrabber
%ignore setGrabber(shared_ptr<ofBaseVideoPlayer>);
%ignore getGrabber();
%ignore getGrabber() const;

%include "video/ofVideoGrabber.h"

// ----- ofVideoPlayer.h -----

// DIFF: ofVideoPlayer.h: ignore getPlayer/setPlayer
%ignore setPlayer(shared_ptr<ofBaseVideoPlayer>);
%ignore getPlayer();
%ignore getPlayer() const;

%include "video/ofVideoPlayer.h"

// ----- ofQuickTimGrabber.h -----

// not needed

// ----- ofQuickTimePlayer.h -----

// not needed

// ----- ofQTUtils.h -----

// not needed

// ----- ofQTKitGrabber.h -----

// not needed

// ----- ofQTKitPlayer.h -----

// not needed

// ----- ofQTKitMovieRenderer.h -----

// not needed

// ----- ofAVFoundationVideoGrabber.h -----

// not needed

// ----- ofAVFoundationVideoPlayer.h -----

// not needed

// ----- ofAVFoundationPlayer.h -----

// not needed
