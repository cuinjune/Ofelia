// scripting language attributes to add using getters/setters 
// 2015 Dan Wilcox <danomatika@gmail.com>

// ----- ofFbo.h -----

// ATTR: ofFbo.h:
// ATTR:   getter: allocated, texture, depthTexture, width, height,
// ATTR:           numTextures, id, idDrawBuffer, depthBuffer, stencilBuffer
// ATTR:   getter/setter: defaultTexture
%attribute(ofFbo, bool, allocated, isAllocated);
%attribute(ofFbo, int, defaultTexture, getDefaultTextureIndex, setDefaultTextureIndex);
%attribute(ofFbo, ofTexture &, texture, getTexture);
%attribute(ofFbo, ofTexture &, depthTexture, getDepthTexture);
%attribute(ofFbo, float, width, getWidth);
%attribute(ofFbo, float, height, getHeight);
%attribute(ofFbo, int, numTextures, getNumTextures);
%attribute(ofFbo, unsigned int, id, getId);
%attribute(ofFbo, unsigned int, idDrawBuffer, getIdDrawBuffer);
%attribute(ofFbo, unsigned int, depthBuffer, getDepthBuffer);
%attribute(ofFbo, unsigned int, stencilBuffer, getStencilBuffer);

// ----- ofTexture.h -----

// ATTR: ofTexture.h:
// ATTR:   getter: allocated, width, height, usingTextureMatrix, textureData
// ATTR:   getter/setter: textureMatrix
%attribute(ofTexture, bool, allocated, isAllocated);
%attribute(ofTexture, float, width, getWidth);
%attribute(ofTexture, float, height, getHeight);
%attribute(ofTexture, ofMatrix4x4 &, textureMatrix, getTextureMatrix, setTextureMatrix);
%attribute(ofTexture, bool, usingTextureMatrix, isUsingTextureMatrix);
%attribute(ofTexture, ofTextureData &, textureData, getTextureData);

// ----- ofImage.h -----

// ATTR: ofImage.h:
// ATTR:   getter: allocated, texture, pixels, width, height, imageType
// ATTR:   getter/setter: usingTexture
%attribute(ofImage_<unsigned char>, bool, allocated, isAllocated);
%attribute(ofImage_<unsigned char>, bool, usingTexture, isUsingTexture, setUseTexture);
%attribute(ofImage_<unsigned char>, ofTexture &, texture, getTexture);
%attribute(ofImage_<unsigned char>, ofPixels &, pixels, getPixels);
%attribute(ofImage_<unsigned char>, float, width, getWidth);
%attribute(ofImage_<unsigned char>, float, height, getHeight);
%attribute(ofImage_<unsigned char>, ofImageType, imageType, getImageType, setImageType);

%attribute(ofImage_<float>, bool, allocated, isAllocated);
%attribute(ofImage_<float>, bool, usingTexture, isUsingTexture, setUseTexture);
%attribute(ofImage_<float>, ofTexture &, texture, getTexture);
%attribute(ofImage_<float>, ofPixels &, pixels, getPixels);
%attribute(ofImage_<float>, float, width, getWidth);
%attribute(ofImage_<float>, float, height, getHeight);
%attribute(ofImage_<float>, ofImageType, imageType, getImageType, setImageType);

%attribute(ofImage_<unsigned short>, bool, allocated, isAllocated);
%attribute(ofImage_<unsigned short>, bool, usingTexture, isUsingTexture, setUseTexture);
%attribute(ofImage_<unsigned short>, ofTexture &, texture, getTexture);
%attribute(ofImage_<unsigned short>, ofPixels &, pixels, getPixels);
%attribute(ofImage_<unsigned short>, float, width, getWidth);
%attribute(ofImage_<unsigned short>, float, height, getHeight);
%attribute(ofImage_<unsigned short>, ofImageType, imageType, getImageType, setImageType);

// ----- ofSoundStream.h -----

// ATTR: ofSoundStream.h:
// ATTR:   getter: tickCount, numInputChannels, numOutputChannels,
// ATTR:           sampleRate, bufferSize
%attribute(ofSoundStream, unsigned long, tickCount, getTickCount);
%attribute(ofSoundStream, int, numInputChannels, getNumInputChannels);
%attribute(ofSoundStream, int, numOutputChannels, getNumOutputChannels);
%attribute(ofSoundStream, int, sampleRate, getSampleRate);
%attribute(ofSoundStream, int, bufferSize, getBufferSize);

// ----- ofSoundPlayer.h -----

// ATTR: ofSoundPlayer.h:
// ATTR:   getter: playing, loaded
// ATTR:   getter/setter: volume, pan, speed, position, positionMS
%attribute(ofSoundPlayer, float, volume, getVolume, setVolume);
%attribute(ofSoundPlayer, float, pan, getPan, setPan);
%attribute(ofSoundPlayer, float, speed, getSpeed, setSpeed);
%attribute(ofSoundPlayer, float, position, getPosition, setPosition);
%attribute(ofSoundPlayer, int, positionMS, getPositionMS, setPositionMS);
%attribute(ofSoundPlayer, bool, playing, isPlaying);
%attribute(ofSoundPlayer, bool, loaded, isLoaded);

// ----- ofFpsCounter.h -----

// ATTR: ofFpsCounter.h: getter: fps, numFrames, lastFrameNanos, lastFrameSecs
%attribute(ofFpsCounter, double, fps, getFps);
%attribute(ofFpsCounter, uint64_t, numFrames, getNumFrames);
%attribute(ofFpsCounter, uint64_t, lastFrameNanos, getLastFrameNanos);
%attribute(ofFpsCounter, double, lastFrameSecs, getLastFrameSecs);

// ----- ofBufferObject.h -----

// ATTR: ofBufferObject.h: getter: allocated, id
%attribute(ofBufferObject, bool, allocated, isAllocated);
%attribute(ofBufferObject, unsigned int, id, getId);

// ----- ofPixels.h -----

// ATTR: ofPixels.h: getter: width & height
%attribute(ofPixels_<unsigned char>, float, width, getWidth);
%attribute(ofPixels_<unsigned char>, float, height, getHeight);

%attribute(ofPixels_<float>, float, width, getWidth);
%attribute(ofPixels_<float>, float, height, getHeight);

%attribute(ofPixels_<unsigned short>, float, width, getWidth);
%attribute(ofPixels_<unsigned short>, float, height, getHeight);

// ----- ofTrueTypeFont.h -----

// ATTR: ofTrueTypeFont.h: getter/setter: lineHeight, letterSpacing, & spaceSize
%attribute(ofTrueTypeFont, float, lineHeight, getLineHeight, setLineHeight);
%attribute(ofTrueTypeFont, float, letterSpacing, getLetterSpacing, setLetterSpacing);
%attribute(ofTrueTypeFont, float, spaceSize, getSpaceSize, setSpaceSize);

// ----- ofFileUtils.h -----

// ATTR: ofFileUtils.h: ofBuffer getter: length, data, text
%attribute(ofBuffer, long, length, size);
%attribute(ofBuffer, char *, data, getData);
%attributestring(ofBuffer, std::string, text, getText);

// ----- ofVideoGrabber.h -----

// ATTR: ofVideoGrabber.h:
// ATTR:   getter: frameNew, pixelFormat, pixels, texture,
// ATTR:           width, height, initialized
// ATTR:   getter/setter: usingTexture
%attribute(ofVideoGrabber, bool, frameNew, isFrameNew);
%attribute(ofVideoGrabber, ofPixelFormat, pixelFormat, getPixelFormat);
%attribute(ofVideoGrabber, ofPixels &, pixels, getPixels);
%attribute(ofVideoGrabber, ofTexture &, texture, getTexture);
%attribute(ofVideoGrabber, bool, usingTexture, isUsingTexture, setUseTexture);
%attribute(ofVideoGrabber, float, width, getWidth);
%attribute(ofVideoGrabber, float, height, getHeight);
%attribute(ofVideoGrabber, bool, initialized, isInitialized);

// ----- ofVideoPlayer.h -----

// ATTR: ofVideoPlayer.h:
// ATTR:   getter: frameNew, pixels, movieDone, texture, width,
// ATTR:           height, duration, loaded, playing, initialized, numFrames
// ATTR:   getter/setter: usingTexture, pixelFormat, position,
// ATTR:                  speed, loopState, paused, frame
%attributestring(ofVideoPlayer, std::string, moviePath, getMoviePath);
%attribute(ofVideoPlayer, ofPixelFormat, pixelFormat, getPixelFormat, setPixelFormat);
%attribute(ofVideoPlayer, bool, frameNew, isFrameNew);
%attribute(ofVideoPlayer, ofPixels &, pixels, getPixels);
%attribute(ofVideoPlayer, float, position, getPosition, setPosition);
%attribute(ofVideoPlayer, float, speed, getSpeed, setSpeed);
%attribute(ofVideoPlayer, float, duration, getDuration);
%attribute(ofVideoPlayer, ofLoopType, loopState, getLoopState, setLoopState);
%attribute(ofVideoPlayer, bool, movieDone, getIsMovieDone);
%attribute(ofVideoPlayer, bool, usingTexture, isUsingTexture, setUseTexture);
%attribute(ofVideoPlayer, ofTexture &, texture, getTexture);
%attribute(ofVideoPlayer, int, frame, getCurrentFrame, setFrame);
%attribute(ofVideoPlayer, int, numFrames, getTotalNumFrames);
%attribute(ofVideoPlayer, float, width, getWidth);
%attribute(ofVideoPlayer, float, height, getHeight);
%attribute(ofVideoPlayer, bool, paused, isPaused, setPaused);
%attribute(ofVideoPlayer, bool, loaded, isLoaded);
%attribute(ofVideoPlayer, bool, playing, isPlaying);
%attribute(ofVideoPlayer, bool, initialized, isInitialized);
