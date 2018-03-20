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

#include "ofeliaOrien.h"
#include "ofeliaListeners.h"
#include "ofeliaGetters.h"

/* ________________________________________________________________________________
 * public class methods
 */
ofeliaOrien::ofeliaOrien()
{
    /* initialize variables */
    bWindowOwner = false;
    lockMode = ORIEN_ROTATION_LOCK_FREE;
    orientation = ORIEN_ORIENTATION_DEFAULT;
    forceVertical = ORIEN_FORCE_DEFAULT;
    forceHorizontal = ORIEN_FORCE_90_LEFT;
    animationTime = 0.0f;
    degree = 0.0f;
    targetDegree = 0.0f;
    distFactor = 0.0f;
    animatedDegree = 0.0f;
    bShouldAnimate = false;
    blockSendingOrienToPd = false;
    setRotationPoint();
}
ofeliaOrien::~ofeliaOrien()
{
}

void ofeliaOrien::updateRotation()
{
    /* update device rotation degree */
    if (bShouldAnimate)
        degree = getAnimatedRotationDegree();
}

/* this should be placed after setting the window scale and before all drawings */
void ofeliaOrien::drawRotation()
{
    /* apply rotation */
    ofGetCurrentRenderer()->translate(centerPoint);
    ofGetCurrentRenderer()->rotate(degree);
    ofGetCurrentRenderer()->translate(restorePoint);
    ofGetCurrentRenderer()->pushMatrix();
}

/* this should be placed after all drawings */
void ofeliaOrien::drawMasking()
{
    ofGetCurrentRenderer()->popMatrix();
    
    /* draw masking area while rotating */
    if (bShouldAnimate) {
        
        ofGetCurrentRenderer()->pushMatrix();
        int dWidth = deviceWidth;
        int dHeight = deviceHeight;
        float diagonal = sqrtf(powf(dWidth, 2.0f) + powf(dHeight, 2.0f));
        float frameWidth = diagonal - dWidth;
        float frameHeight = diagonal - dHeight;
        ofPath path;
        path.setFillColor(ofColor::black);
        path.rectangle(-frameWidth*0.5f, -frameHeight*0.5f, dWidth+frameWidth, dHeight+frameHeight);
        path.rectangle(0.0f, 0.0f, dWidth, dHeight);
        path.draw();
        ofGetCurrentRenderer()->popMatrix();
    }
    /* restore rotation */
    ofGetCurrentRenderer()->translate(-restorePoint);
    ofGetCurrentRenderer()->rotate(-degree);
    ofGetCurrentRenderer()->translate(-centerPoint);
}

void ofeliaOrien::rotateToNearestTargetDeg(const int newOrientation,
                                           const float aniTime)
{
    float targetDeg;
    
    switch (newOrientation) {
            
        case ORIEN_ORIENTATION_DEFAULT:
            targetDeg = 0.0f;
            break;
        case ORIEN_ORIENTATION_RIGHT:
            swap(centerPoint.x, centerPoint.y);
            targetDeg = -90.0f;
            break;
        case ORIEN_ORIENTATION_INVERT:
            targetDeg = -180.0f;
            break;
        case ORIEN_ORIENTATION_LEFT:
            swap(centerPoint.x, centerPoint.y);
            targetDeg = 90.0f;
            break;
        default:
            break;
    }
    const float nearestTargetDeg = ofLerpDegrees(degree, targetDeg, 1.0f);
    setRotationAnimation(degree, nearestTargetDeg, aniTime);
}

void ofeliaOrien::setDeviceRotationLockMode(const int newLockMode,
                                               int &currentOrientation)
{
    if (lockMode == newLockMode)
        return;
    float doubleAnimationTime = animationTime * 2;
    
    if (lockMode == ORIEN_ROTATION_LOCK_FREE) {
        
        /* if the current orientation is either 90_RIGHT or 90_LEFT */
        if (currentOrientation%2) {
            
            swap(deviceWidth, deviceHeight);
            setRotationPoint();
            const float nearestTargetDeg = ofLerpDegrees(degree, 0.0f, 1.0f);
            setRotationAnimation(degree, nearestTargetDeg, animationTime);
        }
        /* if the current orientation is 180 and the lock mode will be set to HARD */
        else if (currentOrientation == ORIEN_ORIENTATION_INVERT &&
                 newLockMode == ORIEN_ROTATION_LOCK_HARD) {
            
            /* flip back to default */
            rotateToNearestTargetDeg(ORIEN_ORIENTATION_DEFAULT, doubleAnimationTime);
            currentOrientation = ORIEN_ORIENTATION_DEFAULT;
        }
    }
    /* if the lock mode is set to either SOFT or HARD */
    else {
        
        if (newLockMode == ORIEN_ROTATION_LOCK_FREE) {

            /* if the actual orientation is either 90_RIGHT or 90_LEFT */
            if (orientation%2) {
                
                swap(deviceWidth, deviceHeight);
                setRotationPoint();
                rotateToNearestTargetDeg(orientation, animationTime);
            }
            /* if the actual orientation is either DEFAULT or 180 */
            else {
                
                rotateToNearestTargetDeg(orientation, doubleAnimationTime);
            }
        }
        else if (newLockMode == ORIEN_ROTATION_LOCK_SOFT) {
            
            /* if the actual orientation is either DEFAULT or 180 */
            if (orientation%2 == 0)
                rotateToNearestTargetDeg(orientation, doubleAnimationTime);
        }
        /* if the current orientation is 180 and the lock mode will be set to HARD */
        else if (currentOrientation == ORIEN_ORIENTATION_INVERT &&
                 newLockMode == ORIEN_ROTATION_LOCK_HARD) {
            
            /* flip back to default */
            rotateToNearestTargetDeg(ORIEN_ORIENTATION_DEFAULT, doubleAnimationTime);
            currentOrientation = ORIEN_ORIENTATION_DEFAULT;
        }
    }
    lockMode = newLockMode;
    sendDimenToPd(deviceWidth, deviceHeight);
    
    if (currentOrientation != orientation) {
        
        orientation = currentOrientation;
        sendOrienToPd(orientation);
    }
}

void ofeliaOrien::setDeviceOrientation(int &newOrientation)
{
    if (orientation == newOrientation)
        return;
    float doubleAnimationTime = animationTime * 2;
    
    if (lockMode == ORIEN_ROTATION_LOCK_FREE) {
        
        /* if the new orientation has different dimensions */
        if (orientation%2 != newOrientation%2) {
            
            swap(deviceWidth, deviceHeight);
            setRotationPoint();
            rotateToNearestTargetDeg(newOrientation, animationTime);
        }
        /* if the new orientation is an opposite side to the current one */
        else {
            
            setRotationPoint();
            rotateToNearestTargetDeg(newOrientation, doubleAnimationTime);
        }
    }
    /* if the lock mode is set to either SOFT or HARD */
    else {

        setRotationPoint();
        
        /* if the new orientation is either DEFAULT or 180 */
        if (newOrientation%2 == 0) {
            
            if (lockMode == ORIEN_ROTATION_LOCK_SOFT)
                rotateToNearestTargetDeg(newOrientation, doubleAnimationTime);
        }
    }
    orientation = newOrientation;
    sendDimenToPd(deviceWidth, deviceHeight);
    sendOrienToPd(orientation);
}

int ofeliaOrien::getDeviceRotationLockMode()
{
    return lockMode;
}

int ofeliaOrien::getDeviceOrientation()
{
    return orientation;
}

/* ________________________________________________________________________________
 * private class methods
 */
void ofeliaOrien::setRotationPoint()
{
    const int widthCenter = static_cast<int>(deviceWidth * 0.5f);
    const int heightCenter = static_cast<int>(deviceHeight * 0.5f);
    centerPoint.x = widthCenter;
    centerPoint.y = heightCenter;
    restorePoint.x = -widthCenter;
    restorePoint.y = -heightCenter;
}

void ofeliaOrien::setRotationAnimation(const float beginDegree,
                                       const float endDegree,
                                       const float timeMillis)
{
    if (beginDegree != endDegree) {
        
        animatedDegree = beginDegree;
        targetDegree = endDegree;
        isIncreasing = beginDegree < endDegree;
        distFactor = (endDegree - beginDegree) / timeMillis;
        bShouldAnimate = true;
    }
    else {
        
        degree = targetDegree = endDegree;
        bShouldAnimate = false;
    }
}

float ofeliaOrien::getAnimatedRotationDegree()
{
    double lastFrameTime = ofGetLastFrameTime() * 1000;
    float distPerFrame = distFactor * lastFrameTime;
    animatedDegree += distPerFrame;
    
    if ((isIncreasing && animatedDegree >= targetDegree) ||
        (!isIncreasing && animatedDegree <= targetDegree)) {

        animatedDegree = targetDegree;
        bShouldAnimate = false;
    }
    return animatedDegree;
}

void ofeliaOrien::sendDimenToPd(const int width, const int height)
{
    if (bWindowOwner) {
        
        if (width == lastDeviceWidth && height == lastDeviceHeight)
            return;
        value_setfloat(t_ofeliaGetWidth::getWidthSym, static_cast<t_float>(width));
        value_setfloat(t_ofeliaGetHeight::getHeightSym, static_cast<t_float>(height));
        lastDeviceWidth = width;
        lastDeviceHeight = height;
    }
}

void ofeliaOrien::sendOrienToPd(const int newOrientation)
{
    if (bWindowOwner && !blockSendingOrienToPd) {
        
        value_setfloat(t_ofeliaGetOrien::getOrienSym, static_cast<t_float>(newOrientation));
        
        if (t_ofeliaOrienListener::orienListenerSym->s_thing)
            pd_float(t_ofeliaOrienListener::orienListenerSym->s_thing, static_cast<t_float>(newOrientation));
    }
}

