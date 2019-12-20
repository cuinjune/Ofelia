#include "ofMain.h"
#include "ofApp.h"

int main()
{
	ofGLESWindowSettings settings;
    settings.setGLESVersion(1);
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
	return 0;
}


#ifdef TARGET_ANDROID
void ofAndroidApplicationInit()
{
    //application scope init
}

void ofAndroidActivityInit()
{
    //activity scope init
    main();
}
#endif
