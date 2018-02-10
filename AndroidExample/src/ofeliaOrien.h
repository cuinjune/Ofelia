/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#ifndef ofeliaOrien_h
#define ofeliaOrien_h

#pragma once

#include "ofeliaBase.h"

class ofeliaOrien {
    
public:
    
    ofeliaOrien();
    ~ofeliaOrien();

    /* class methods */
    void updateRotation();
    void drawRotation();
    void drawMasking();
    void rotateToNearestTargetDeg(const int newOrientation,
                                  const float aniTime);
    void setDeviceRotationLockMode(const int newLockMode,
                                   int &currentOrientation);
    void setDeviceOrientation(int &newOrientation);
    int getDeviceRotationLockMode();
    int getDeviceOrientation();
    void sendDimenToPd(const int width, const int height);
    void sendOrienToPd(const int newOrientation);
    
    /* class variables */
    bool bWindowOwner;
    int deviceWidth;
    int deviceHeight;
    ofVec2f centerPoint;
    ofVec2f restorePoint;
    float degree;
    bool blockSendingOrienToPd;
    int lastDeviceWidth;
    int lastDeviceHeight;
    float animationTime;
    
private:
    
    /* enumerations */
    enum RotationLock {
        
        ORIEN_ROTATION_LOCK_FREE,
        ORIEN_ROTATION_LOCK_SOFT,
        ORIEN_ROTATION_LOCK_HARD
    };
    enum Orientation {
        
        ORIEN_ORIENTATION_DEFAULT,
        ORIEN_ORIENTATION_RIGHT,
        ORIEN_ORIENTATION_INVERT,
        ORIEN_ORIENTATION_LEFT
    };
    enum ForceVertical {
        
        ORIEN_FORCE_DEFAULT,
        ORIEN_FORCE_180
    };
    enum ForceHorizontal {
        
        ORIEN_FORCE_90_LEFT,
        ORIEN_FORCE_90_RIGHT
    };
    
    /* class methods */
    void setRotationPoint();
    void setRotationAnimation(const float beginDegree,
                              const float endDegree,
                              const float timeMillis);
    float getAnimatedRotationDegree();
    
    /* class variables */
    int lockMode;
    int orientation;
    bool forceVertical;
    bool forceHorizontal;
    float targetDegree;
    float distFactor;
    float animatedDegree;
    bool isIncreasing;
    bool bShouldAnimate;
};


#endif /* ofeliaOrien_h */
