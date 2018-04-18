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

#include "ofeliaPolyline.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadPolyline2d::objName = "ofLoadPolyline2d";
unsigned int t_ofeliaLoadPolyline2d::counter;
vector<t_ofeliaLoadPolyline2dData> t_ofeliaLoadPolyline2d::polylineData;
vector<t_ofeliaLoadPolylineSet> t_ofeliaLoadPolyline2d::polylineSets;
bool t_ofeliaLoadPolyline2d::bInited;
const char *t_ofeliaLoadPolyline3d::objName = "ofLoadPolyline3d";
unsigned int t_ofeliaLoadPolyline3d::counter;
vector<t_ofeliaLoadPolyline3dData> t_ofeliaLoadPolyline3d::polylineData;
vector<t_ofeliaLoadPolylineSet> t_ofeliaLoadPolyline3d::polylineSets;
bool t_ofeliaLoadPolyline3d::bInited;
const char *t_ofeliaDrawPolyline2d::objName = "ofDrawPolyline2d";
const char *t_ofeliaDrawPolyline3d::objName = "ofDrawPolyline3d";
const char *t_ofeliaDoesPolyline2dNameExist::objName = "ofDoesPolyline2dNameExist";
const char *t_ofeliaDoesPolyline3dNameExist::objName = "ofDoesPolyline3dNameExist";
const char *t_ofeliaEditPolyline2dPoint::objName = "ofEditPolyline2dPoint";
const char *t_ofeliaEditPolyline3dPoint::objName = "ofEditPolyline3dPoint";
const char *t_ofeliaGetPolyline2dPoint::objName = "ofGetPolyline2dPoint";
const char *t_ofeliaGetPolyline3dPoint::objName = "ofGetPolyline3dPoint";
const char *t_ofeliaGetPolyline2dPoints::objName = "ofGetPolyline2dPoints";
const char *t_ofeliaGetPolyline3dPoints::objName = "ofGetPolyline3dPoints";
const char *t_ofeliaIsPointInsidePolyline2d::objName = "ofIsPointInsidePolyline2d";
const char *t_ofeliaIsPointInsidePolyline3d::objName = "ofIsPointInsidePolyline3d";
const char *t_ofeliaGetPolyline2dCommand::objName = "ofGetPolyline2dCommand";
const char *t_ofeliaGetPolyline3dCommand::objName = "ofGetPolyline3dCommand";
const char *t_ofeliaGetPolyline2dCommands::objName = "ofGetPolyline2dCommands";
const char *t_ofeliaGetPolyline3dCommands::objName = "ofGetPolyline3dCommands";
const char *t_ofeliaGetPolyline2dBoundingBox::objName = "ofGetPolyline2dBoundingBox";
const char *t_ofeliaGetPolyline3dBoundingBox::objName = "ofGetPolyline3dBoundingBox";
const char *t_ofeliaGetPolyline2dCentroid::objName = "ofGetPolyline2dCentroid";
const char *t_ofeliaGetPolyline3dCentroid::objName = "ofGetPolyline3dCentroid";
const char *t_ofeliaGetPolyline2dArea::objName = "ofGetPolyline2dArea";
const char *t_ofeliaGetPolyline3dArea::objName = "ofGetPolyline3dArea";
const char *t_ofeliaGetPolyline2dPerimeter::objName = "ofGetPolyline2dPerimeter";
const char *t_ofeliaGetPolyline3dPerimeter::objName = "ofGetPolyline3dPerimeter";

/* ________________________________________________________________________________
 * common methods
 */
void getLoadPolylineCmdTypeString(LoadPolylineCmdType type, t_string &name)
{
    switch (type) {
            
        case POLYLINE_LOAD_CMDTYPE_NEWSUBPATH:
            name = "newSubPath";
            break;
        case POLYLINE_LOAD_CMDTYPE_VERTEX:
            name = "vertex";
            break;
        case POLYLINE_LOAD_CMDTYPE_MOVETO:
            name = "moveTo";
            break;
        case POLYLINE_LOAD_CMDTYPE_LINETO:
            name = "lineTo";
            break;
        case POLYLINE_LOAD_CMDTYPE_ARC:
            name = "arc";
            break;
        case POLYLINE_LOAD_CMDTYPE_CURVETO:
            name = "curveTo";
            break;
        case POLYLINE_LOAD_CMDTYPE_BEZIERTO:
            name = "bezierTo";
            break;
        case POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO:
            name = "quadBezierTo";
            break;
        case POLYLINE_LOAD_CMDTYPE_CLOSE:
            name = "close";
            break;
        case POLYLINE_LOAD_CMDTYPE_CIRCLE:
            name = "circle";
            break;
        case POLYLINE_LOAD_CMDTYPE_ELLIPSE:
            name = "ellipse";
            break;
        case POLYLINE_LOAD_CMDTYPE_SECTOR:
            name = "sector";
            break;
        case POLYLINE_LOAD_CMDTYPE_TRIANGLE:
            name = "triangle";
            break;
        case POLYLINE_LOAD_CMDTYPE_EQTRIANGLE:
            name = "eqTriangle";
            break;
        case POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE:
            name = "isoTriangle";
            break;
        case POLYLINE_LOAD_CMDTYPE_QUAD:
            name = "quad";
            break;
        case POLYLINE_LOAD_CMDTYPE_SQUARE:
            name = "square";
            break;
        case POLYLINE_LOAD_CMDTYPE_RECTANGLE:
            name = "rectangle";
            break;
        case POLYLINE_LOAD_CMDTYPE_RECTROUNDED:
            name = "rectRounded";
            break;
        case POLYLINE_LOAD_CMDTYPE_RECTROUNDED4:
            name = "rectRounded4";
            break;
        case POLYLINE_LOAD_CMDTYPE_CROSS:
            name = "cross";
            break;
        case POLYLINE_LOAD_CMDTYPE_HEART:
            name = "heart";
            break;
        case POLYLINE_LOAD_CMDTYPE_MOON:
            name = "moon";
            break;
        case POLYLINE_LOAD_CMDTYPE_REGPOLYGON:
            name = "regPolygon";
            break;
        case POLYLINE_LOAD_CMDTYPE_STAR:
            name = "star";
            break;
        default:
            break;
    }
}

inline void printRectMode(ofRectMode rectMode)
{
    t_string mode;
    
    if (rectMode == OF_RECTMODE_CORNER)
        mode = "CORNER";
    else if (rectMode == OF_RECTMODE_CENTER)
        mode = "CENTER";
    post("rectMode : %s", mode.c_str());
}

void updatePolyline2dProperties(const int pos)
{
    ofPolyline polylineMerged;
    float perimeterMerged = 0.0f;
    
    for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i) {
        
        polylineMerged.addVertices(t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).getVertices());
        perimeterMerged += t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).getPerimeter();
    }
    getBoundingBoxDimen2d(t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen, polylineMerged.getVertices());
    t_ofeliaLoadPolyline2d::polylineData[pos].centroid = polylineMerged.getCentroid2D();
    t_ofeliaLoadPolyline2d::polylineData[pos].area = polylineMerged.getArea();
    t_ofeliaLoadPolyline2d::polylineData[pos].perimeter = perimeterMerged;
    t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = false;
}

void updatePolyline3dProperties(const int pos)
{
    ofPolyline polylineMerged;
    float perimeterMerged = 0.0f;
    
    for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i) {
        
        polylineMerged.addVertices(t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).getVertices());
        perimeterMerged += t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).getPerimeter();
    }
    getBoundingBoxDimen3d(t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen, polylineMerged.getVertices());
    t_ofeliaLoadPolyline3d::polylineData[pos].centroid = polylineMerged.getCentroid2D();
    t_ofeliaLoadPolyline3d::polylineData[pos].area = polylineMerged.getArea();
    t_ofeliaLoadPolyline3d::polylineData[pos].perimeter = perimeterMerged;
    t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = false;
}

/* ________________________________________________________________________________
 * ofLoadPolyline2d object methods
 */
void ofeliaLoadPolyline2d_clear(t_ofeliaLoadPolyline2d *x);
void ofeliaLoadPolyline2d_vecSizeOut(t_ofeliaLoadPolyline2d *x);

int getPositionByPolyline2dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadPolyline2d::polylineData), end(t_ofeliaLoadPolyline2d::polylineData),
                            [&objID](const t_ofeliaLoadPolyline2dData &polylineData)
                            {
                                return polylineData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadPolyline2d::polylineData.begin());
}

bool setLoadPolyline2dElemType(const char *name, t_ofeliaLoadPolyline2dElem &elem)
{
    if (!strcmp(name, "newSubPath"))  {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_NEWSUBPATH;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "vertex")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moveTo"))  {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_MOVETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "lineTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_LINETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "arc")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ARC;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "curveTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CURVETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "bezierTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_BEZIERTO;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "quadBezierTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "close")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CLOSE;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "triangle") || !strcmp(name, "tri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_TRIANGLE;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "quad")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_QUAD;
        elem.numArgs = 8;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_HEART;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_MOON;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadPolyline2d::objName, name);
        return 0;
    }
    return 1;
}

void setPathFromLoadPolyline2dElems(t_ofeliaLoadPolyline2d *x,
                                    unique_ptr<ofPath> &path,
                                    vector<t_ofeliaLoadPolyline2dElem> &elems)
{
    ofVec2f curPos;
    
    for (size_t i=0; i<elems.size(); ++i) {
        
        switch (elems[i].type) {
                
            case POLYLINE_LOAD_CMDTYPE_NEWSUBPATH:
            {
                path->newSubPath();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_VERTEX:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->lineTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_MOVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->moveTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_LINETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->lineTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ARC:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[1],
                             elems[i].args[2], elems[i].args[3],
                             elems[i].args[2] < elems[i].args[3], x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CURVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->curveTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_BEZIERTO:
            {
                curPos.set(elems[i].args[4], elems[i].args[5]);
                path->bezierTo(ofVec2f(elems[i].args[0], elems[i].args[1]),
                               ofVec2f(elems[i].args[2], elems[i].args[3]),
                               curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO:
            {
                curPos.set(elems[i].args[4], elems[i].args[5]);
                path->quadBezierTo(ofVec2f(elems[i].args[0], elems[i].args[1]),
                                   ofVec2f(elems[i].args[2], elems[i].args[3]),
                                   curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CLOSE:
            {
                path->close();
                
                if (!path->getOutline().empty())
                    curPos = path->getOutline().back().getVertices()[0];
                else
                    curPos.set(0.0f, 0.0f);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CIRCLE:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[0], 0.0f, 360.0f,
                             true, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ELLIPSE:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0]*0.5f, elems[i].args[1]*0.5f, 0.0f, 360.0f,
                             true, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_SECTOR:
            {
                ofPolyline polyline;
                polyline.addVertex(curPos);
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[1],
                             elems[i].args[2], elems[i].args[3],
                             elems[i].args[2] < elems[i].args[3], x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_TRIANGLE:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->triangle(curPos.x, curPos.y, 0.0f,
                               elems[i].args[2], elems[i].args[3], 0.0f,
                               elems[i].args[4], elems[i].args[5], 0.0f);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_EQTRIANGLE:
            {
                const float halfSize = elems[i].args[0] * 0.5f;
                const float inRadius = elems[i].args[0] * ofeliaConstants::heightFactor / 3;
                path->triangle(curPos.x, curPos.y-inRadius*2.0f, 0.0f,
                               curPos.x+halfSize, curPos.y+inRadius, 0.0f,
                               curPos.x-halfSize, curPos.y+inRadius, 0.0f);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE:
            {
                const float halfWidth = elems[i].args[0] * 0.5f;
                const float halfHeight = elems[i].args[1] * 0.5f;
                path->triangle(curPos.x, curPos.y-halfHeight, 0.0f,
                               curPos.x+halfWidth, curPos.y+halfHeight, 0.0f,
                               curPos.x-halfWidth, curPos.y+halfHeight, 0.0f);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_QUAD:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->moveTo(curPos);
                path->lineTo(ofVec2f(elems[i].args[2], elems[i].args[3]));
                path->lineTo(ofVec2f(elems[i].args[4], elems[i].args[5]));
                path->lineTo(ofVec2f(elems[i].args[6], elems[i].args[7]));
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_SQUARE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER) {
                    
                    const float halfSize = elems[i].args[0] * 0.5f;
                    curPos -= ofVec2f(halfSize, halfSize);
                }
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[0]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTANGLE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[1]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTROUNDED:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, 0.0f,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[2],
                                  elems[i].args[2], elems[i].args[2]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTROUNDED4:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, 0.0f,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[3],
                                  elems[i].args[4], elems[i].args[5]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CROSS:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float sHalf = elems[i].args[0] * 0.5f;
                const float tHalf = elems[i].args[1] * 0.5f;
                const float cX_tHalf_add = centerX + tHalf;
                const float cX_tHalf_sub = centerX - tHalf;
                const float cX_sHalf_add = centerX + sHalf;
                const float cX_sHalf_sub = centerX - sHalf;
                const float cY_tHalf_add = centerY + tHalf;
                const float cY_tHalf_sub = centerY - tHalf;
                const float cY_sHalf_add = centerY + sHalf;
                const float cY_sHalf_sub = centerY - sHalf;
                path->moveTo(ofVec2f(cX_tHalf_sub, cY_sHalf_sub));
                path->lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_sub));
                path->lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_sub));
                path->lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_sub));
                path->lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_add));
                path->lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_add));
                path->lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_add));
                path->lineTo(ofVec2f(cX_tHalf_sub, cY_sHalf_add));
                path->lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_add));
                path->lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_add));
                path->lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_sub));
                path->lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_sub));
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_HEART:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float width = elems[i].args[0];
                const float height = elems[i].args[1];
                const float widthStretched = width * 1.125f;
                const float heightHalf = height * 0.5f;
                const ofVec2f beginEnd(centerX, centerY - height * 0.25f);
                const ofVec2f bezierP1(centerX, centerY - height * 0.75f);
                const ofVec2f bezierP3(centerX, centerY + heightHalf);
                path->moveTo(beginEnd);
                path->bezierTo(bezierP1,
                               ofVec2f(centerX+widthStretched, centerY+5.0f-heightHalf),
                               bezierP3);
                path->moveTo(beginEnd);
                path->bezierTo(bezierP1,
                               ofVec2f(centerX-widthStretched, centerY+5.0f-heightHalf),
                               bezierP3);
                path->moveTo(bezierP3);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_MOON:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float radiusX = elems[i].args[0] * 0.5f;
                const float radiusY = elems[i].args[1] * 0.5f;
                ofPolyline polyline;
                polyline.arc(ofVec2f(centerX, centerY),
                             elems[i].args[2], radiusY,
                             -90.0f, 90.0f, true, x->circleRes);
                polyline.arc(ofVec2f(centerX, centerY),
                             radiusX, radiusY,
                             90.0f, -90.0f, false, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_REGPOLYGON:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float radius = elems[i].args[0];
                const int numSides = static_cast<int>(elems[i].args[1]);
                const float divAngle = 360.0f / numSides;
                const float beginAngle = 90.0f - divAngle * 0.5f;
                float posX, posY;
                getCoordinatesFromRadAngle(posX, posY, radius, beginAngle);
                path->moveTo(ofVec2f(centerX+posX, centerY+posY));
                
                for (int i=1; i<numSides; ++i) {
                    
                    getCoordinatesFromRadAngle(posX, posY, radius, beginAngle + divAngle*i);
                    path->lineTo(ofVec2f(centerX+posX, centerY+posY));
                }
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_STAR:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const int sideNum = static_cast<int>(elems[i].args[2]) * 2;
                const float divAngle = 360.0f / sideNum;
                const float radius[] = {elems[i].args[0], elems[i].args[1]};
                const float beginAngle = 90.0f - divAngle;
                float posX, posY;
                getCoordinatesFromRadAngle(posX, posY, radius[0], beginAngle);
                path->moveTo(ofVec2f(centerX+posX, centerY+posY));
                
                for (int i=1; i<sideNum; ++i) {
                    
                    getCoordinatesFromRadAngle(posX, posY, radius[i%2], beginAngle + divAngle*i);
                    path->lineTo(ofVec2f(centerX+posX, centerY+posY));
                }
                path->close();
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

bool getLoadPolyline2dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPolyline2dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadPolyline2dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPolyline2d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPolyline2d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPolyline2d::objName);
        return 0;
    }
    return 1;
}

bool getLoadPolyline2dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadPolyline2dElem> &elems)
{
    t_string name;
    t_ofeliaLoadPolyline2dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadPolyline2dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPolyline2d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPolyline2d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPolyline2dCmdData &cmd)
{
    switch (argc) {
            
        case 0:
        {
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                cmd.fromIndex = cmd.toIndex = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                cmd.fromIndex = truncf(argv[0].a_w.w_float);
                cmd.toIndex = truncf(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadPolyline2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadPolyline2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline2d *x = reinterpret_cast<t_ofeliaLoadPolyline2d*>(pd_new(ofeliaLoadPolyline2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadPolyline2dData polylineData;
    getVarNameLocalPrefixes(polylineData.varName);
    vector<t_ofeliaLoadPolyline2dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {

            polylineData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(polylineData.varName);
            
            if (--argc) {
                
                if (!getLoadPolyline2dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->rectMode = OF_RECTMODE_CORNER;
    x->circleRes = x->curveRes = OFELIA_DEFAULT_RESOLUTION;
    x->simplify = 0.0f;
    x->objID = polylineData.objID = t_ofeliaLoadPolyline2d::counter++;
    t_ofeliaLoadPolyline2d::polylineData.push_back(polylineData);
    t_ofeliaLoadPolyline2d::polylineSets.push_back({make_unique<ofPath>(),
                                                make_unique<vector<ofPolyline>>()});
    x->vecSize = 0;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    x->numPoints_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadPolyline2d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadPolyline2d_init(t_ofeliaLoadPolyline2d *x)
{
    if (!t_ofeliaLoadPolyline2d::bInited && x->bInitGate) {
        
        t_ofeliaLoadPolyline2d::polylineSets.clear();

        for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineData.size(); ++i)
            t_ofeliaLoadPolyline2d::polylineSets.push_back({make_unique<ofPath>(),
                                                        make_unique<vector<ofPolyline>>()});
        t_ofeliaLoadPolyline2d::bInited = true;
    }
}

void ofeliaLoadPolyline2d_update(t_ofeliaLoadPolyline2d *x)
{
    const int pos = getPositionByPolyline2dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadPolyline2d::polylineData[pos].elems.size());
            const auto elemsBegin = t_ofeliaLoadPolyline2d::polylineData[pos].elems.begin();
            const t_ofeliaLoadPolyline2dElem &elem = x->cmdVec[i].elem;
            
            switch (x->cmdVec[i].state) {
                    
                case POLYLINE_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadPolyline2d::polylineData[pos].elems.insert(elemsBegin + fromIndex,
                                                                        insertSize, elem);
                    break;
                }
                case POLYLINE_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadPolyline2d::polylineData[pos].elems.erase(elemsBegin + fromIndex,
                                                                           elemsBegin + toIndex);
                    }
                    break;
                }
                case POLYLINE_LOAD_CMD_FILL:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        fill(elemsBegin + fromIndex, elemsBegin + toIndex, elem);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        x->shouldOutlet = true;
    }
    if (x->shouldOutlet) {
        
        t_ofeliaLoadPolyline2d::polylineSets[pos].path->clear();
        t_ofeliaLoadPolyline2d::polylineSets[pos].path->setCircleResolution(x->circleRes);
        t_ofeliaLoadPolyline2d::polylineSets[pos].path->setCurveResolution(x->curveRes);
        setPathFromLoadPolyline2dElems(x, t_ofeliaLoadPolyline2d::polylineSets[pos].path, t_ofeliaLoadPolyline2d::polylineData[pos].elems);
        t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->operator=(t_ofeliaLoadPolyline2d::polylineSets[pos].path->getOutline());
        
        if (x->simplify != 0.0f) {
            
            for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i)
                t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).simplify(x->simplify);
        }
        /* find point indices of the polyline */
        t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices.clear();
        
        for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i) {
            
            const size_t numVertices = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).getVertices().size();
            
            for (size_t j=0; j<numVertices; ++j)
                t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices.push_back(make_pair(i, j));
        }
        t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
        
        /* output number of points and current size of the polyline */
        x->numPoints = static_cast<int>(t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices.size());
        x->vecSize = static_cast<int>(t_ofeliaLoadPolyline2d::polylineData[pos].elems.size());
        ofeliaLoadPolyline2d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadPolyline2d_exit(t_ofeliaLoadPolyline2d *x)
{
    if (t_ofeliaLoadPolyline2d::bInited)
        t_ofeliaLoadPolyline2d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline2d_vecSizeOut(t_ofeliaLoadPolyline2d *x)
{
    outlet_float(x->numPoints_out, static_cast<t_float>(x->numPoints));
    outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadPolyline2d_name(t_ofeliaLoadPolyline2d *x, t_symbol *s)
{
    const int pos = getPositionByPolyline2dObjID(x->objID);
    t_ofeliaLoadPolyline2d::polylineData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadPolyline2d::polylineData[pos].varName);
}

void ofeliaLoadPolyline2d_rectMode(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaLoadPolyline2d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPolyline2d_circleRes(t_ofeliaLoadPolyline2d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: circleRes out of range", t_ofeliaLoadPolyline2d::objName);
        return;
    }
    x->circleRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline2d_curveRes(t_ofeliaLoadPolyline2d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: curveRes out of range", t_ofeliaLoadPolyline2d::objName);
        return;
    }
    x->curveRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline2d_simplify(t_ofeliaLoadPolyline2d *x, t_floatarg f)
{
    const float simplify = f;
    
    if (simplify < 0.0f) {
        
        error("%s: simplify out of range", t_ofeliaLoadPolyline2d::objName);
        return;
    }
    x->simplify = simplify;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline2d_load(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline2dElem> elems;
    
    if (getLoadPolyline2dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadPolyline2d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline2d_add(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline2dCmdData cmd;
    
    if (getLoadPolyline2dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = POLYLINE_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPolyline2d_append(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline2dElem> elems;
    
    if (getLoadPolyline2dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline2d_prepend(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline2dElem> elems;
    
    if (getLoadPolyline2dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline2d_insert(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline2dCmdData cmd;
    
    if (getLoadPolyline2dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline2d_fill(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline2dCmdData cmd;
    
    if (getLoadPolyline2dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = POLYLINE_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline2d_erase(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline2dCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = POLYLINE_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPolyline2d_clear(t_ofeliaLoadPolyline2d *x)
{
    t_ofeliaLoadPolyline2dCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = POLYLINE_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadPolyline2d_set(t_ofeliaLoadPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadPolyline2d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadPolyline2d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline2d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPolyline2d::objName);
    }
}

void ofeliaLoadPolyline2d_print(t_ofeliaLoadPolyline2d *x)
{
    post("\n[%s]", t_ofeliaLoadPolyline2d::objName);
    const int pos = getPositionByPolyline2dObjID(x->objID);
    post("name : %s", t_ofeliaLoadPolyline2d::polylineData[pos].varName.name.c_str());
    printRectMode(x->rectMode);
    post("circleRes : %d", x->circleRes);
    post("curveRes : %d", x->curveRes);
    post("simplify : %g", x->simplify);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineData[pos].elems.size(); ++i) {
        
        stringstream ss;
        const char space = ' ';
        t_string name;
        getLoadPolylineCmdTypeString(t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].type, name);
        ss << name;
        
        for (size_t j=0; j<t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].numArgs; ++j)
            ss << space << t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].args[j];
        post(" %s", ss.str().c_str());
    }
}

void ofeliaLoadPolyline2d_free(t_ofeliaLoadPolyline2d *x)
{
    const int pos = getPositionByPolyline2dObjID(x->objID);
    t_ofeliaLoadPolyline2d::polylineData.erase(t_ofeliaLoadPolyline2d::polylineData.begin() + pos);
    t_ofeliaLoadPolyline2d::polylineSets.erase(t_ofeliaLoadPolyline2d::polylineSets.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->vecSize_out);
    outlet_free(x->numPoints_out);
}

void ofeliaLoadPolyline2d_setup()
{
    ofeliaLoadPolyline2d_class = class_new(gensym("ofLoadPolyline2d"),
                                           reinterpret_cast<t_newmethod>(ofeliaLoadPolyline2d_new),
                                           reinterpret_cast<t_method>(ofeliaLoadPolyline2d_free),
                                           sizeof(t_ofeliaLoadPolyline2d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_rectMode),
                    gensym("rectMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_circleRes),
                    gensym("circleRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_curveRes),
                    gensym("curveRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_simplify),
                    gensym("simplify"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline2d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLoadPolyline3d object methods
 */
void ofeliaLoadPolyline3d_clear(t_ofeliaLoadPolyline3d *x);
void ofeliaLoadPolyline3d_vecSizeOut(t_ofeliaLoadPolyline3d *x);

int getPositionByPolyline3dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadPolyline3d::polylineData), end(t_ofeliaLoadPolyline3d::polylineData),
                            [&objID](const t_ofeliaLoadPolyline3dData &polylineData)
                            {
                                return polylineData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadPolyline3d::polylineData.begin());
}

bool setLoadPolyline3dElemType(const char *name, t_ofeliaLoadPolyline3dElem &elem)
{
    if (!strcmp(name, "newSubPath"))  {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_NEWSUBPATH;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "vertex")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "moveTo"))  {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_MOVETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "lineTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_LINETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "arc")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ARC;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "curveTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CURVETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "bezierTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_BEZIERTO;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "quadBezierTo")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "close")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CLOSE;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "triangle") || !strcmp(name, "tri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_TRIANGLE;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "quad")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_QUAD;
        elem.numArgs = 12;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_HEART;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_MOON;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = POLYLINE_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadPolyline3d::objName, name);
        return 0;
    }
    return 1;
}

void setPathFromLoadPolyline3dElems(t_ofeliaLoadPolyline3d *x,
                                    unique_ptr<ofPath> &path,
                                    vector<t_ofeliaLoadPolyline3dElem> &elems)
{
    ofVec3f curPos;
    
    for (size_t i=0; i<elems.size(); ++i) {
        
        switch (elems[i].type) {
                
            case POLYLINE_LOAD_CMDTYPE_NEWSUBPATH:
            {
                path->newSubPath();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_VERTEX:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->lineTo(ofVec3f(curPos));
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_MOVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->moveTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_LINETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->lineTo(ofVec3f(curPos));
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ARC:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[1],
                             elems[i].args[2], elems[i].args[3],
                             elems[i].args[2] < elems[i].args[3], x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CURVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->curveTo(curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_BEZIERTO:
            {
                curPos.set(elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                path->bezierTo(ofVec3f(elems[i].args[0], elems[i].args[1], elems[i].args[2]),
                               ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]),
                               curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO:
            {
                curPos.set(elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                path->quadBezierTo(ofVec3f(elems[i].args[0], elems[i].args[1], elems[i].args[2]),
                                   ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]),
                                   curPos);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CLOSE:
            {
                path->close();
                
                if (!path->getOutline().empty())
                    curPos = path->getOutline().back().getVertices()[0];
                else
                    curPos.set(0.0f, 0.0f, 0.0f);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CIRCLE:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[0], 0.0f, 360.0f,
                             true, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ELLIPSE:
            {
                ofPolyline polyline;
                polyline.arc(curPos,
                             elems[i].args[0]*0.5f, elems[i].args[1]*0.5f, 0.0f, 360.0f,
                             true, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_SECTOR:
            {
                ofPolyline polyline;
                polyline.addVertex(curPos);
                polyline.arc(curPos,
                             elems[i].args[0], elems[i].args[1],
                             elems[i].args[2], elems[i].args[3],
                             elems[i].args[2] < elems[i].args[3], x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_TRIANGLE:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->triangle(curPos.x, curPos.y, curPos.z,
                               elems[i].args[3], elems[i].args[4], elems[i].args[5],
                               elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_EQTRIANGLE:
            {
                const float halfSize = elems[i].args[0] * 0.5f;
                const float inRadius = elems[i].args[0] * ofeliaConstants::heightFactor / 3;
                path->triangle(curPos.x, curPos.y-inRadius*2.0f, curPos.z,
                               curPos.x+halfSize, curPos.y+inRadius, curPos.z,
                               curPos.x-halfSize, curPos.y+inRadius, curPos.z);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE:
            {
                const float halfWidth = elems[i].args[0] * 0.5f;
                const float halfHeight = elems[i].args[1] * 0.5f;
                path->triangle(curPos.x, curPos.y-halfHeight, curPos.z,
                               curPos.x+halfWidth, curPos.y+halfHeight, curPos.z,
                               curPos.x-halfWidth, curPos.y+halfHeight, curPos.z);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_QUAD:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->moveTo(curPos);
                path->lineTo(ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]));
                path->lineTo(ofVec3f(elems[i].args[6], elems[i].args[7], elems[i].args[8]));
                path->lineTo(ofVec3f(elems[i].args[9], elems[i].args[10], elems[i].args[11]));
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_SQUARE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER) {
                    
                    const float halfSize = elems[i].args[0] * 0.5f;
                    curPos -= ofVec3f(halfSize, halfSize);
                }
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[0]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTANGLE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[1]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTROUNDED:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, curPos.z,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[2],
                                  elems[i].args[2], elems[i].args[2]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_RECTROUNDED4:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, curPos.z,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[3],
                                  elems[i].args[4], elems[i].args[5]);
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_CROSS:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float centerZ = curPos.z;
                const float sHalf = elems[i].args[0] * 0.5f;
                const float tHalf = elems[i].args[1] * 0.5f;
                const float cX_tHalf_add = centerX + tHalf;
                const float cX_tHalf_sub = centerX - tHalf;
                const float cX_sHalf_add = centerX + sHalf;
                const float cX_sHalf_sub = centerX - sHalf;
                const float cY_tHalf_add = centerY + tHalf;
                const float cY_tHalf_sub = centerY - tHalf;
                const float cY_sHalf_add = centerY + sHalf;
                const float cY_sHalf_sub = centerY - sHalf;
                path->moveTo(ofVec3f(cX_tHalf_sub, cY_sHalf_sub, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_add, cY_sHalf_sub, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_add, cY_tHalf_sub, centerZ));
                path->lineTo(ofVec3f(cX_sHalf_add, cY_tHalf_sub, centerZ));
                path->lineTo(ofVec3f(cX_sHalf_add, cY_tHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_add, cY_tHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_add, cY_sHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_sub, cY_sHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_sub, cY_tHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_sHalf_sub, cY_tHalf_add, centerZ));
                path->lineTo(ofVec3f(cX_sHalf_sub, cY_tHalf_sub, centerZ));
                path->lineTo(ofVec3f(cX_tHalf_sub, cY_tHalf_sub, centerZ));
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_HEART:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float centerZ = curPos.z;
                const float width = elems[i].args[0];
                const float height = elems[i].args[1];
                const float widthStretched = width * 1.125f;
                const float heightHalf = height * 0.5f;
                const ofVec3f beginEnd(centerX, centerY - height * 0.25f, centerZ);
                const ofVec3f bezierP1(centerX, centerY - height * 0.75f, centerZ);
                const ofVec3f bezierP3(centerX, centerY + heightHalf, centerZ);
                path->moveTo(beginEnd);
                path->bezierTo(bezierP1,
                               ofVec3f(centerX+widthStretched, centerY+5.0f-heightHalf, centerZ),
                               bezierP3);
                path->moveTo(beginEnd);
                path->bezierTo(bezierP1,
                               ofVec3f(centerX-widthStretched, centerY+5.0f-heightHalf, centerZ),
                               bezierP3);
                path->moveTo(bezierP3);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_MOON:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float centerZ = curPos.z;
                const float radiusX = elems[i].args[0] * 0.5f;
                const float radiusY = elems[i].args[1] * 0.5f;
                ofPolyline polyline;
                polyline.arc(ofVec3f(centerX, centerY, centerZ),
                             elems[i].args[2], radiusY,
                             -90.0f, 90.0f, true, x->circleRes);
                polyline.arc(ofVec3f(centerX, centerY, centerZ),
                             radiusX, radiusY,
                             90.0f, -90.0f, false, x->circleRes);
                path->moveTo(polyline.getVertices()[0]);
                
                for (size_t i=1; i<polyline.getVertices().size(); ++i)
                    path->lineTo(polyline.getVertices()[i]);
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_REGPOLYGON:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float centerZ = curPos.z;
                const float radius = elems[i].args[0];
                const int numSides = static_cast<int>(elems[i].args[1]);
                const float divAngle = 360.0f / numSides;
                const float beginAngle = 90.0f - divAngle * 0.5f;
                float posX, posY;
                getCoordinatesFromRadAngle(posX, posY, radius, beginAngle);
                path->moveTo(ofVec3f(centerX+posX, centerY+posY, centerZ));
                
                for (int i=1; i<numSides; ++i) {
                    
                    getCoordinatesFromRadAngle(posX, posY, radius, beginAngle + divAngle*i);
                    path->lineTo(ofVec3f(centerX+posX, centerY+posY, centerZ));
                }
                path->close();
                break;
            }
            case POLYLINE_LOAD_CMDTYPE_STAR:
            {
                const float centerX = curPos.x;
                const float centerY = curPos.y;
                const float centerZ = curPos.z;
                const int sideNum = static_cast<int>(elems[i].args[2]) * 2;
                const float divAngle = 360.0f / sideNum;
                const float radius[] = {elems[i].args[0], elems[i].args[1]};
                const float beginAngle = 90.0f - divAngle;
                float posX, posY;
                getCoordinatesFromRadAngle(posX, posY, radius[0], beginAngle);
                path->moveTo(ofVec3f(centerX+posX, centerY+posY, centerZ));
                
                for (int i=1; i<sideNum; ++i) {
                    
                    getCoordinatesFromRadAngle(posX, posY, radius[i%2], beginAngle + divAngle*i);
                    path->lineTo(ofVec3f(centerX+posX, centerY+posY, centerZ));
                }
                path->close();
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

bool getLoadPolyline3dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPolyline3dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadPolyline3dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPolyline3d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPolyline3d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPolyline3d::objName);
        return 0;
    }
    return 1;
}

bool getLoadPolyline3dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadPolyline3dElem> &elems)
{
    t_string name;
    t_ofeliaLoadPolyline3dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadPolyline3dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPolyline3d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPolyline3d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPolyline3dCmdData &cmd)
{
    switch (argc) {
            
        case 0:
        {
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                cmd.fromIndex = cmd.toIndex = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                cmd.fromIndex = truncf(argv[0].a_w.w_float);
                cmd.toIndex = truncf(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadPolyline3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadPolyline3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline3d *x = reinterpret_cast<t_ofeliaLoadPolyline3d*>(pd_new(ofeliaLoadPolyline3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadPolyline3dData polylineData;
    getVarNameLocalPrefixes(polylineData.varName);
    vector<t_ofeliaLoadPolyline3dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {

            polylineData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(polylineData.varName);
            
            if (--argc) {
                
                if (!getLoadPolyline3dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->rectMode = OF_RECTMODE_CORNER;
    x->circleRes = x->curveRes = OFELIA_DEFAULT_RESOLUTION;
    x->simplify = 0.0f;
    x->objID = polylineData.objID = t_ofeliaLoadPolyline3d::counter++;
    t_ofeliaLoadPolyline3d::polylineData.push_back(polylineData);
    t_ofeliaLoadPolyline3d::polylineSets.push_back({make_unique<ofPath>(),
                                                make_unique<vector<ofPolyline>>()});
    x->vecSize = 0;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    x->numPoints_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadPolyline3d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadPolyline3d_init(t_ofeliaLoadPolyline3d *x)
{
    if (!t_ofeliaLoadPolyline3d::bInited && x->bInitGate) {
        
        t_ofeliaLoadPolyline3d::polylineSets.clear();

        for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineData.size(); ++i)
            t_ofeliaLoadPolyline3d::polylineSets.push_back({make_unique<ofPath>(),
                make_unique<vector<ofPolyline>>()});
        t_ofeliaLoadPolyline3d::bInited = true;
    }
}

void ofeliaLoadPolyline3d_update(t_ofeliaLoadPolyline3d *x)
{
    const int pos = getPositionByPolyline3dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadPolyline3d::polylineData[pos].elems.size());
            const auto elemsBegin = t_ofeliaLoadPolyline3d::polylineData[pos].elems.begin();
            const t_ofeliaLoadPolyline3dElem &elem = x->cmdVec[i].elem;
            
            switch (x->cmdVec[i].state) {
                    
                case POLYLINE_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadPolyline3d::polylineData[pos].elems.insert(elemsBegin + fromIndex,
                                                                        insertSize, elem);
                    break;
                }
                case POLYLINE_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadPolyline3d::polylineData[pos].elems.erase(elemsBegin + fromIndex,
                                                                           elemsBegin + toIndex);
                    }
                    break;
                }
                case POLYLINE_LOAD_CMD_FILL:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        fill(elemsBegin + fromIndex, elemsBegin + toIndex, elem);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        x->shouldOutlet = true;
    }
    if (x->shouldOutlet) {
        
        t_ofeliaLoadPolyline3d::polylineSets[pos].path->clear();
        t_ofeliaLoadPolyline3d::polylineSets[pos].path->setCircleResolution(x->circleRes);
        t_ofeliaLoadPolyline3d::polylineSets[pos].path->setCurveResolution(x->curveRes);
        setPathFromLoadPolyline3dElems(x, t_ofeliaLoadPolyline3d::polylineSets[pos].path, t_ofeliaLoadPolyline3d::polylineData[pos].elems);
        t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->operator=(t_ofeliaLoadPolyline3d::polylineSets[pos].path->getOutline());
        
        if (x->simplify != 0.0f) {
            
            for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i)
                t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).simplify(x->simplify);
        }
        /* find point indices of the polyline */
        t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices.clear();
        
        for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i) {
            
            const size_t numVertices = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).getVertices().size();
            
            for (size_t j=0; j<numVertices; ++j)
                t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices.push_back(make_pair(i, j));
        }
        t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
        
        /* output number of points and current size of the polyline */
        x->numPoints = static_cast<int>(t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices.size());
        x->vecSize = static_cast<int>(t_ofeliaLoadPolyline3d::polylineData[pos].elems.size());
        ofeliaLoadPolyline3d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadPolyline3d_exit(t_ofeliaLoadPolyline3d *x)
{
    if (t_ofeliaLoadPolyline3d::bInited)
        t_ofeliaLoadPolyline3d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline3d_vecSizeOut(t_ofeliaLoadPolyline3d *x)
{
    outlet_float(x->numPoints_out, static_cast<t_float>(x->numPoints));
    outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadPolyline3d_name(t_ofeliaLoadPolyline3d *x, t_symbol *s)
{
    const int pos = getPositionByPolyline3dObjID(x->objID);
    t_ofeliaLoadPolyline3d::polylineData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadPolyline3d::polylineData[pos].varName);
}

void ofeliaLoadPolyline3d_rectMode(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaLoadPolyline3d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPolyline3d_circleRes(t_ofeliaLoadPolyline3d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: circleRes out of range", t_ofeliaLoadPolyline3d::objName);
        return;
    }
    x->circleRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline3d_curveRes(t_ofeliaLoadPolyline3d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: curveRes out of range", t_ofeliaLoadPolyline3d::objName);
        return;
    }
    x->curveRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline3d_simplify(t_ofeliaLoadPolyline3d *x, t_floatarg f)
{
    const float simplify = f;
    
    if (simplify < 0.0f) {
        
        error("%s: simplify out of range", t_ofeliaLoadPolyline3d::objName);
        return;
    }
    x->simplify = simplify;
    x->shouldOutlet = true;
}

void ofeliaLoadPolyline3d_load(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline3dElem> elems;
    
    if (getLoadPolyline3dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadPolyline3d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline3d_add(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline3dCmdData cmd;
    
    if (getLoadPolyline3dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = POLYLINE_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPolyline3d_append(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline3dElem> elems;
    
    if (getLoadPolyline3dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline3d_prepend(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPolyline3dElem> elems;
    
    if (getLoadPolyline3dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPolyline3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline3d_insert(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline3dCmdData cmd;
    
    if (getLoadPolyline3dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = POLYLINE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline3d_fill(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline3dCmdData cmd;
    
    if (getLoadPolyline3dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = POLYLINE_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPolyline3d_erase(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPolyline3dCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = POLYLINE_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPolyline3d_clear(t_ofeliaLoadPolyline3d *x)
{
    t_ofeliaLoadPolyline3dCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = POLYLINE_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadPolyline3d_set(t_ofeliaLoadPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadPolyline3d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadPolyline3d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPolyline3d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPolyline3d::objName);
    }
}

void ofeliaLoadPolyline3d_print(t_ofeliaLoadPolyline3d *x)
{
    post("\n[%s]", t_ofeliaLoadPolyline3d::objName);
    const int pos = getPositionByPolyline3dObjID(x->objID);
    post("name : %s", t_ofeliaLoadPolyline3d::polylineData[pos].varName.name.c_str());
    printRectMode(x->rectMode);
    post("circleRes : %d", x->circleRes);
    post("curveRes : %d", x->curveRes);
    post("simplify : %g", x->simplify);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineData[pos].elems.size(); ++i) {
        
        stringstream ss;
        const char space = ' ';
        t_string name;
        getLoadPolylineCmdTypeString(t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].type, name);
        ss << name;
        
        for (size_t j=0; j<t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].numArgs; ++j)
            ss << space << t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].args[j];
        post(" %s", ss.str().c_str());
    }
}

void ofeliaLoadPolyline3d_free(t_ofeliaLoadPolyline3d *x)
{
    const int pos = getPositionByPolyline3dObjID(x->objID);
    t_ofeliaLoadPolyline3d::polylineData.erase(t_ofeliaLoadPolyline3d::polylineData.begin() + pos);
    t_ofeliaLoadPolyline3d::polylineSets.erase(t_ofeliaLoadPolyline3d::polylineSets.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->vecSize_out);
    outlet_free(x->numPoints_out);
}

void ofeliaLoadPolyline3d_setup()
{
    ofeliaLoadPolyline3d_class = class_new(gensym("ofLoadPolyline3d"),
                                           reinterpret_cast<t_newmethod>(ofeliaLoadPolyline3d_new),
                                           reinterpret_cast<t_method>(ofeliaLoadPolyline3d_free),
                                           sizeof(t_ofeliaLoadPolyline3d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_rectMode),
                    gensym("rectMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_circleRes),
                    gensym("circleRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_curveRes),
                    gensym("curveRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_simplify),
                    gensym("simplify"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPolyline3d_class, reinterpret_cast<t_method>(ofeliaLoadPolyline3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByPolyline2dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadPolyline2d::polylineData), end(t_ofeliaLoadPolyline2d::polylineData),
                      [&name] (const t_ofeliaLoadPolyline2dData &polylineData) {
                          return (polylineData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadPolyline2d::polylineData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadPolyline2d::polylineData));
}

int getPositionByPolyline3dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadPolyline3d::polylineData), end(t_ofeliaLoadPolyline3d::polylineData),
                      [&name] (const t_ofeliaLoadPolyline3dData &polylineData) {
                          return (polylineData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadPolyline3d::polylineData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadPolyline3d::polylineData));
}

/* ________________________________________________________________________________
 * ofDrawPolyline2d object methods
 */
void *ofeliaDrawPolyline2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawPolyline2d *x = reinterpret_cast<t_ofeliaDrawPolyline2d*>(pd_new(ofeliaDrawPolyline2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPolyline2d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawPolyline2d_bang(t_ofeliaDrawPolyline2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByPolyline2dName(name);
            
            if (pos != -1) {
                
                if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                    
                    if (!x->elem.width && !x->elem.height) {
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i)
                            t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).draw();
                    }
                    else {
                        
                        if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                            updatePolyline2dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.y;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, 1.0f);
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i)
                            t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).draw();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawPolyline2d::objName);
    }
}

void ofeliaDrawPolyline2d_name(t_ofeliaDrawPolyline2d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawPolyline2d_width(t_ofeliaDrawPolyline2d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawPolyline2d_height(t_ofeliaDrawPolyline2d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawPolyline2d_set(t_ofeliaDrawPolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPolyline2d::objName);
}

void ofeliaDrawPolyline2d_print(t_ofeliaDrawPolyline2d *x)
{
    post("\n[%s]", t_ofeliaDrawPolyline2d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height;
    
    if (!x->elem.width && !x->elem.height) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByPolyline2dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline2dProperties(pos);
                width = t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.x;
                height = t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.y;
            }
            else {
                
                width = 0.0f;
                height = 0.0f;
            }
        }
        else {
            
            width = 0.0f;
            height = 0.0f;
        }
    }
    else {
        
        width = x->elem.width;
        height = x->elem.height;
    }
    post("name : %s", name.c_str());
    post("width : %g", width);
    post("height : %g", height);
}

void ofeliaDrawPolyline2d_setup()
{
    ofeliaDrawPolyline2d_class = class_new(gensym("ofDrawPolyline2d"),
                                           reinterpret_cast<t_newmethod>(ofeliaDrawPolyline2d_new),
                                           0, sizeof(t_ofeliaDrawPolyline2d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_bang));
    class_addmethod(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawPolyline2d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawPolyline3d object methods
 */
void *ofeliaDrawPolyline3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawPolyline3d *x = reinterpret_cast<t_ofeliaDrawPolyline3d*>(pd_new(ofeliaDrawPolyline3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPolyline3d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("depth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawPolyline3d_bang(t_ofeliaDrawPolyline3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByPolyline3dName(name);
            
            if (pos != -1) {
                
                if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                    
                    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i)
                            t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).draw();
                    }
                    else {
                        
                        if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                            updatePolyline3dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.y;
                        const float depthScale = x->elem.depth / t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.z;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, depthScale);
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i)
                            t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).draw();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawPolyline3d::objName);
    }
}

void ofeliaDrawPolyline3d_name(t_ofeliaDrawPolyline3d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawPolyline3d_width(t_ofeliaDrawPolyline3d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawPolyline3d_height(t_ofeliaDrawPolyline3d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawPolyline3d_depth(t_ofeliaDrawPolyline3d *x, t_floatarg f)
{
    x->elem.depth = f;
}

void ofeliaDrawPolyline3d_set(t_ofeliaDrawPolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPolyline3d::objName);
}

void ofeliaDrawPolyline3d_print(t_ofeliaDrawPolyline3d *x)
{
    post("\n[%s]", t_ofeliaDrawPolyline3d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height, depth;
    
    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByPolyline3dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline3dProperties(pos);
                width = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.x;
                height = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.y;
                depth = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.z;
            }
            else {
                
                width = 0.0f;
                height = 0.0f;
                depth = 0.0f;
            }
        }
        else {
            
            width = 0.0f;
            height = 0.0f;
            depth = 0.0f;
        }
    }
    else {
        
        width = x->elem.width;
        height = x->elem.height;
        depth = x->elem.depth;
    }
    post("name : %s", name.c_str());
    post("width : %g", width);
    post("height : %g", height);
    post("depth : %g", depth);
}

void ofeliaDrawPolyline3d_setup()
{
    ofeliaDrawPolyline3d_class = class_new(gensym("ofDrawPolyline3d"),
                                           reinterpret_cast<t_newmethod>(ofeliaDrawPolyline3d_new),
                                           0, sizeof(t_ofeliaDrawPolyline3d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_bang));
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_depth),
                    gensym("depth"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawPolyline3d_class, reinterpret_cast<t_method>(ofeliaDrawPolyline3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesPolyline2dNameExist object methods
 */
void *ofeliaDoesPolyline2dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesPolyline2dNameExist *x = reinterpret_cast<t_ofeliaDoesPolyline2dNameExist*>(pd_new(ofeliaDoesPolyline2dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesPolyline2dNameExist_bang(t_ofeliaDoesPolyline2dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByPolyline2dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesPolyline2dNameExist::objName);
    }
}

void ofeliaDoesPolyline2dNameExist_set(t_ofeliaDoesPolyline2dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesPolyline2dNameExist_print(t_ofeliaDoesPolyline2dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesPolyline2dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesPolyline2dNameExist_setup()
{
    ofeliaDoesPolyline2dNameExist_class = class_new(gensym("ofDoesPolyline2dNameExist"),
                                                   reinterpret_cast<t_newmethod>(ofeliaDoesPolyline2dNameExist_new),
                                                   0, sizeof(t_ofeliaDoesPolyline2dNameExist),
                                                   CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesPolyline2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline2dNameExist_bang));
    class_addmethod(ofeliaDoesPolyline2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline2dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPolyline2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline2dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPolyline2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline2dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesPolyline3dNameExist object methods
 */
void *ofeliaDoesPolyline3dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesPolyline3dNameExist *x = reinterpret_cast<t_ofeliaDoesPolyline3dNameExist*>(pd_new(ofeliaDoesPolyline3dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesPolyline3dNameExist_bang(t_ofeliaDoesPolyline3dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByPolyline3dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesPolyline3dNameExist::objName);
    }
}

void ofeliaDoesPolyline3dNameExist_set(t_ofeliaDoesPolyline3dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesPolyline3dNameExist_print(t_ofeliaDoesPolyline3dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesPolyline3dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesPolyline3dNameExist_setup()
{
    ofeliaDoesPolyline3dNameExist_class = class_new(gensym("ofDoesPolyline3dNameExist"),
                                                   reinterpret_cast<t_newmethod>(ofeliaDoesPolyline3dNameExist_new),
                                                   0, sizeof(t_ofeliaDoesPolyline3dNameExist),
                                                   CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesPolyline3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline3dNameExist_bang));
    class_addmethod(ofeliaDoesPolyline3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline3dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPolyline3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline3dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPolyline3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPolyline3dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditPolyline2dPoint object methods
 */
void *ofeliaEditPolyline2dPoint_new(t_symbol *s)
{
    t_ofeliaEditPolyline2dPoint *x = reinterpret_cast<t_ofeliaEditPolyline2dPoint*>(pd_new(ofeliaEditPolyline2dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditPolyline2dPoint_float(t_ofeliaEditPolyline2dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditPolyline2dPoint_assign(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_assignX(t_ofeliaEditPolyline2dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.x = f;
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_assignY(t_ofeliaEditPolyline2dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.y = f;
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_add(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_sub(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_mult(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_div(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_lerp(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        point.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_limit(t_ofeliaEditPolyline2dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.limit(f);
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_map(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        point.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f),
                                  ofVec3f(argv[2].a_w.w_float, argv[3].a_w.w_float, 0.0f),
                                  ofVec3f(argv[4].a_w.w_float, argv[5].a_w.w_float, 0.0f),
                                  ofVec3f(0.0f, 0.0f, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_middle(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        point.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_normalize(t_ofeliaEditPolyline2dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.normalize();
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_perpendicular(t_ofeliaEditPolyline2dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                /* ofVec2f::perpendicular() */
                const float length = static_cast<float>(sqrt(point.x * point.x + point.y * point.y));
                
                if (length > 0) {
                    
                    const float _x = point.x;
                    point.x = -(point.y / length);
                    point.y = _x/length;
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_rotate(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = point.x * cos(a) - point.y * sin(a);
                            point.y = point.x * sin(a) + point.y * cos(a);
                            point.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = ((point.x-pivot.x)*cos(a) - (point.y-pivot.y)*sin(a)) + pivot.x;
                            point.y = ((point.x-pivot.x)*sin(a) + (point.y-pivot.y)*cos(a)) + pivot.y;
                            point.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_rotateRad(t_ofeliaEditPolyline2dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = angle;
                            float xrot = point.x * cos(a) - point.y * sin(a);
                            point.y = point.x * sin(a) + point.y * cos(a);
                            point.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = angle;
                            const float xrot = ((point.x-pivot.x)*cos(a) - (point.y-pivot.y)*sin(a)) + pivot.x;
                            point.y = ((point.x-pivot.x)*sin(a) + (point.y-pivot.y)*cos(a)) + pivot.y;
                            point.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline2dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline2dPoint::objName);
                        return;
                    }
                }
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_scale(t_ofeliaEditPolyline2dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.scale(f);
                t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline2dPoint::objName);
    }
}

void ofeliaEditPolyline2dPoint_set(t_ofeliaEditPolyline2dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditPolyline2dPoint_print(t_ofeliaEditPolyline2dPoint *x)
{
    post("\n[%s]", t_ofeliaEditPolyline2dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditPolyline2dPoint_setup()
{
    ofeliaEditPolyline2dPoint_class = class_new(gensym("ofEditPolyline2dPoint"),
                                                reinterpret_cast<t_newmethod>(ofeliaEditPolyline2dPoint_new),
                                                0, sizeof(t_ofeliaEditPolyline2dPoint),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_float));
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline2dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditPolyline3dPoint object methods
 */
void *ofeliaEditPolyline3dPoint_new(t_symbol *s)
{
    t_ofeliaEditPolyline3dPoint *x = reinterpret_cast<t_ofeliaEditPolyline3dPoint*>(pd_new(ofeliaEditPolyline3dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditPolyline3dPoint_float(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditPolyline3dPoint_assign(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point = ofVec3f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point = ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_assignX(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.x = f;
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_assignY(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.y = f;
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_assignZ(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.z = f;
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_add(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point += ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_sub(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point -= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_mult(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point *= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_div(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            point /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            point /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point /= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_cross(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        point.cross(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_lerp(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        point.interpolate(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float), argv[3].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_limit(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.limit(f);
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_map(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 12) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT &&
                        argv[6].a_type == A_FLOAT &&
                        argv[7].a_type == A_FLOAT &&
                        argv[8].a_type == A_FLOAT &&
                        argv[9].a_type == A_FLOAT &&
                        argv[10].a_type == A_FLOAT &&
                        argv[11].a_type == A_FLOAT) {
                        
                        point.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float),
                                  ofVec3f(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float),
                                  ofVec3f(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float),
                                  ofVec3f(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_middle(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        point.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_normalize(t_ofeliaEditPolyline3dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.normalize();
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_perpendicular(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        point.perpendicular(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                    return;
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_rotate(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point.rotate(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            point.rotate(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                      argv[2].a_w.w_float,
                                                                      argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            point.rotate(argv[0].a_w.w_float,
                                         ofVec3f(argv[1].a_w.w_float,
                                                 argv[2].a_w.w_float,
                                                 argv[3].a_w.w_float),
                                         ofVec3f(argv[4].a_w.w_float,
                                                 argv[5].a_w.w_float,
                                                 argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_rotateRad(t_ofeliaEditPolyline3dPoint *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            point.rotateRad(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            point.rotateRad(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                         argv[2].a_w.w_float,
                                                                         argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            point.rotateRad(argv[0].a_w.w_float,
                                            ofVec3f(argv[1].a_w.w_float,
                                                    argv[2].a_w.w_float,
                                                    argv[3].a_w.w_float),
                                            ofVec3f(argv[4].a_w.w_float,
                                                    argv[5].a_w.w_float,
                                                    argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditPolyline3dPoint::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditPolyline3dPoint::objName);
                        return;
                    }
                }
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_scale(t_ofeliaEditPolyline3dPoint *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                point.scale(f);
                t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditPolyline3dPoint::objName);
    }
}

void ofeliaEditPolyline3dPoint_set(t_ofeliaEditPolyline3dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditPolyline3dPoint_print(t_ofeliaEditPolyline3dPoint *x)
{
    post("\n[%s]", t_ofeliaEditPolyline3dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditPolyline3dPoint_setup()
{
    ofeliaEditPolyline3dPoint_class = class_new(gensym("ofEditPolyline3dPoint"),
                                                reinterpret_cast<t_newmethod>(ofeliaEditPolyline3dPoint_new),
                                                0, sizeof(t_ofeliaEditPolyline3dPoint),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_float));
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_assignZ),
                    gensym("assignZ"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_cross),
                    gensym("cross"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_perpendicular),
                    gensym("perpendicular"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaEditPolyline3dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dPoint object methods
 */
void *ofeliaGetPolyline2dPoint_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dPoint *x = reinterpret_cast<t_ofeliaGetPolyline2dPoint*>(pd_new(ofeliaGetPolyline2dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline2dPoint_bang(t_ofeliaGetPolyline2dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                const ofVec2f &point = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = point.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = point.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dPoint::objName);
    }
}

void ofeliaGetPolyline2dPoint_float(t_ofeliaGetPolyline2dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPolyline2dPoint_set(t_ofeliaGetPolyline2dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPolyline2dPoint_print(t_ofeliaGetPolyline2dPoint *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPolyline2dPoint_setup()
{
    ofeliaGetPolyline2dPoint_class = class_new(gensym("ofGetPolyline2dPoint"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dPoint_new),
                                               0, sizeof(t_ofeliaGetPolyline2dPoint),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoint_bang));
    class_addfloat(ofeliaGetPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoint_float));
    class_addmethod(ofeliaGetPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dPoint object methods
 */
void *ofeliaGetPolyline3dPoint_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dPoint *x = reinterpret_cast<t_ofeliaGetPolyline3dPoint*>(pd_new(ofeliaGetPolyline3dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline3dPoint_bang(t_ofeliaGetPolyline3dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                const ofVec3f &point = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = point.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = point.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = point.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dPoint::objName);
    }
}

void ofeliaGetPolyline3dPoint_float(t_ofeliaGetPolyline3dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPolyline3dPoint_set(t_ofeliaGetPolyline3dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPolyline3dPoint_print(t_ofeliaGetPolyline3dPoint *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPolyline3dPoint_setup()
{
    ofeliaGetPolyline3dPoint_class = class_new(gensym("ofGetPolyline3dPoint"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dPoint_new),
                                               0, sizeof(t_ofeliaGetPolyline3dPoint),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoint_bang));
    class_addfloat(ofeliaGetPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoint_float));
    class_addmethod(ofeliaGetPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dPoints object methods
 */
void *ofeliaGetPolyline2dPoints_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dPoints *x = reinterpret_cast<t_ofeliaGetPolyline2dPoints*>(pd_new(ofeliaGetPolyline2dPoints_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline2dPoints_bang(t_ofeliaGetPolyline2dPoints *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline2d::polylineData[pos].pointIndices;
                const int ac = static_cast<int>(pointIndices.size()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const int idx = i/2;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dPoints::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dPoints::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dPoints::objName);
    }
}

void ofeliaGetPolyline2dPoints_set(t_ofeliaGetPolyline2dPoints *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dPoints_print(t_ofeliaGetPolyline2dPoints *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dPoints::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dPoints_setup()
{
    ofeliaGetPolyline2dPoints_class = class_new(gensym("ofGetPolyline2dPoints"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dPoints_new),
                                                0, sizeof(t_ofeliaGetPolyline2dPoints),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoints_bang));
    class_addmethod(ofeliaGetPolyline2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoints_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoints_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dPoints_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dPoints object methods
 */
void *ofeliaGetPolyline3dPoints_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dPoints *x = reinterpret_cast<t_ofeliaGetPolyline3dPoints*>(pd_new(ofeliaGetPolyline3dPoints_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline3dPoints_bang(t_ofeliaGetPolyline3dPoints *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPolyline3d::polylineData[pos].pointIndices;
                const int ac = static_cast<int>(pointIndices.size()*3);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=3) {
                    
                    const int idx = i/3;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].y;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].z;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dPoints::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dPoints::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dPoints::objName);
    }
}

void ofeliaGetPolyline3dPoints_set(t_ofeliaGetPolyline3dPoints *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dPoints_print(t_ofeliaGetPolyline3dPoints *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dPoints::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dPoints_setup()
{
    ofeliaGetPolyline3dPoints_class = class_new(gensym("ofGetPolyline3dPoints"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dPoints_new),
                                                0, sizeof(t_ofeliaGetPolyline3dPoints),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoints_bang));
    class_addmethod(ofeliaGetPolyline3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoints_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoints_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dPoints_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsPointInsidePolyline2d object methods
 */
void *ofeliaIsPointInsidePolyline2d_new(t_symbol *s)
{
    t_ofeliaIsPointInsidePolyline2d *x = reinterpret_cast<t_ofeliaIsPointInsidePolyline2d*>(pd_new(ofeliaIsPointInsidePolyline2d_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsPointInsidePolyline2d_list(t_ofeliaIsPointInsidePolyline2d *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float posX = argv[0].a_w.w_float;
                        const float posY = argv[1].a_w.w_float;
                        
                        bool inside = false;
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->size(); ++i) {
                            
                            if (t_ofeliaLoadPolyline2d::polylineSets[pos].polylines->at(i).inside(posX, posY)) {
                                
                                inside = true;
                                break;
                            }
                        }
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsPointInsidePolyline2d::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsPointInsidePolyline2d::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsPointInsidePolyline2d::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsPointInsidePolyline2d::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsPointInsidePolyline2d::objName);
    }
}

void ofeliaIsPointInsidePolyline2d_set(t_ofeliaIsPointInsidePolyline2d *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaIsPointInsidePolyline2d_print(t_ofeliaIsPointInsidePolyline2d *x)
{
    post("\n[%s]", t_ofeliaIsPointInsidePolyline2d::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaIsPointInsidePolyline2d_setup()
{
    ofeliaIsPointInsidePolyline2d_class = class_new(gensym("ofIsPointInsidePolyline2d"),
                                                reinterpret_cast<t_newmethod>(ofeliaIsPointInsidePolyline2d_new),
                                                0, sizeof(t_ofeliaIsPointInsidePolyline2d),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addlist(ofeliaIsPointInsidePolyline2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline2d_list));
    class_addmethod(ofeliaIsPointInsidePolyline2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline2d_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePolyline2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline2d_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePolyline2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsPointInsidePolyline3d object methods
 */
void *ofeliaIsPointInsidePolyline3d_new(t_symbol *s)
{
    t_ofeliaIsPointInsidePolyline3d *x = reinterpret_cast<t_ofeliaIsPointInsidePolyline3d*>(pd_new(ofeliaIsPointInsidePolyline3d_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsPointInsidePolyline3d_list(t_ofeliaIsPointInsidePolyline3d *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float posX = argv[0].a_w.w_float;
                        const float posY = argv[1].a_w.w_float;
                        
                        bool inside = false;
                        
                        for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->size(); ++i) {
                            
                            if (t_ofeliaLoadPolyline3d::polylineSets[pos].polylines->at(i).inside(posX, posY)) {
                                
                                inside = true;
                                break;
                            }
                        }
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsPointInsidePolyline3d::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsPointInsidePolyline3d::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsPointInsidePolyline3d::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsPointInsidePolyline3d::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsPointInsidePolyline3d::objName);
    }
}

void ofeliaIsPointInsidePolyline3d_set(t_ofeliaIsPointInsidePolyline3d *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaIsPointInsidePolyline3d_print(t_ofeliaIsPointInsidePolyline3d *x)
{
    post("\n[%s]", t_ofeliaIsPointInsidePolyline3d::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaIsPointInsidePolyline3d_setup()
{
    ofeliaIsPointInsidePolyline3d_class = class_new(gensym("ofIsPointInsidePolyline3d"),
                                                reinterpret_cast<t_newmethod>(ofeliaIsPointInsidePolyline3d_new),
                                                0, sizeof(t_ofeliaIsPointInsidePolyline3d),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addlist(ofeliaIsPointInsidePolyline3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline3d_list));
    class_addmethod(ofeliaIsPointInsidePolyline3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline3d_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePolyline3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline3d_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePolyline3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePolyline3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dCommand object methods
 */
void *ofeliaGetPolyline2dCommand_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dCommand *x = reinterpret_cast<t_ofeliaGetPolyline2dCommand*>(pd_new(ofeliaGetPolyline2dCommand_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPolyline2dCommand_bang(t_ofeliaGetPolyline2dCommand *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadPolyline2d::polylineData[pos].elems.size())-1);
                vector<t_ofeliaAtomElem> elems;
                t_string name;
                getLoadPolylineCmdTypeString(t_ofeliaLoadPolyline2d::polylineData[pos].elems[index].type, name);
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                
                for (int i=0; i<t_ofeliaLoadPolyline2d::polylineData[pos].elems[index].numArgs; ++i)
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPolyline2d::polylineData[pos].elems[index].args[i]});
                const int ac = static_cast<int>(elems.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dCommand::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dCommand::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dCommand::objName);
    }
}

void ofeliaGetPolyline2dCommand_float(t_ofeliaGetPolyline2dCommand *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPolyline2dCommand_set(t_ofeliaGetPolyline2dCommand *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPolyline2dCommand_print(t_ofeliaGetPolyline2dCommand *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dCommand::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPolyline2dCommand_setup()
{
    ofeliaGetPolyline2dCommand_class = class_new(gensym("ofGetPolyline2dCommand"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dCommand_new),
                                                 0, sizeof(t_ofeliaGetPolyline2dCommand),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dCommand_new),
                     gensym("ofGetPolyline2dCmd"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommand_bang));
    class_addfloat(ofeliaGetPolyline2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommand_float));
    class_addmethod(ofeliaGetPolyline2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommand_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommand_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommand_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dCommand object methods
 */
void *ofeliaGetPolyline3dCommand_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dCommand *x = reinterpret_cast<t_ofeliaGetPolyline3dCommand*>(pd_new(ofeliaGetPolyline3dCommand_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPolyline3dCommand_bang(t_ofeliaGetPolyline3dCommand *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadPolyline3d::polylineData[pos].elems.size())-1);
                vector<t_ofeliaAtomElem> elems;
                t_string name;
                getLoadPolylineCmdTypeString(t_ofeliaLoadPolyline3d::polylineData[pos].elems[index].type, name);
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                
                for (int i=0; i<t_ofeliaLoadPolyline3d::polylineData[pos].elems[index].numArgs; ++i)
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPolyline3d::polylineData[pos].elems[index].args[i]});
                const int ac = static_cast<int>(elems.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dCommand::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dCommand::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dCommand::objName);
    }
}

void ofeliaGetPolyline3dCommand_float(t_ofeliaGetPolyline3dCommand *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPolyline3dCommand_set(t_ofeliaGetPolyline3dCommand *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPolyline3dCommand_print(t_ofeliaGetPolyline3dCommand *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dCommand::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPolyline3dCommand_setup()
{
    ofeliaGetPolyline3dCommand_class = class_new(gensym("ofGetPolyline3dCommand"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dCommand_new),
                                                 0, sizeof(t_ofeliaGetPolyline3dCommand),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dCommand_new),
                     gensym("ofGetPolyline3dCmd"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommand_bang));
    class_addfloat(ofeliaGetPolyline3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommand_float));
    class_addmethod(ofeliaGetPolyline3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommand_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommand_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommand_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dCommands object methods
 */
void *ofeliaGetPolyline2dCommands_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dCommands *x = reinterpret_cast<t_ofeliaGetPolyline2dCommands*>(pd_new(ofeliaGetPolyline2dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPolyline2dCommands_bang(t_ofeliaGetPolyline2dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                LoadPolylineCmdType prevType = POLYLINE_LOAD_CMDTYPE_UNKNOWN;
                
                for (size_t i=0; i<t_ofeliaLoadPolyline2d::polylineData[pos].elems.size(); ++i) {
                    
                    t_string name;
                    const LoadPolylineCmdType type = t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].type;
                    
                    if (type != prevType) {
                        
                        getLoadPolylineCmdTypeString(type, name);
                        elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                        prevType = type;
                    }
                    for (int j=0; j<t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].numArgs; ++j)
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPolyline2d::polylineData[pos].elems[i].args[j]});
                }
                const int ac = static_cast<int>(elems.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dCommands::objName);
    }
}

void ofeliaGetPolyline2dCommands_set(t_ofeliaGetPolyline2dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dCommands_print(t_ofeliaGetPolyline2dCommands *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dCommands_setup()
{
    ofeliaGetPolyline2dCommands_class = class_new(gensym("ofGetPolyline2dCommands"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dCommands_new),
                                                  0, sizeof(t_ofeliaGetPolyline2dCommands),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dCommands_new),
                     gensym("ofGetPolyline2dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommands_bang));
    class_addmethod(ofeliaGetPolyline2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline2dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dCommands object methods
 */
void *ofeliaGetPolyline3dCommands_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dCommands *x = reinterpret_cast<t_ofeliaGetPolyline3dCommands*>(pd_new(ofeliaGetPolyline3dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPolyline3dCommands_bang(t_ofeliaGetPolyline3dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                LoadPolylineCmdType prevType = POLYLINE_LOAD_CMDTYPE_UNKNOWN;
                
                for (size_t i=0; i<t_ofeliaLoadPolyline3d::polylineData[pos].elems.size(); ++i) {
                    
                    t_string name;
                    const LoadPolylineCmdType type = t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].type;
                    
                    if (type != prevType) {
                        
                        getLoadPolylineCmdTypeString(type, name);
                        elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                        prevType = type;
                    }
                    for (int j=0; j<t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].numArgs; ++j)
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPolyline3d::polylineData[pos].elems[i].args[j]});
                }
                const int ac = static_cast<int>(elems.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dCommands::objName);
    }
}

void ofeliaGetPolyline3dCommands_set(t_ofeliaGetPolyline3dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dCommands_print(t_ofeliaGetPolyline3dCommands *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dCommands_setup()
{
    ofeliaGetPolyline3dCommands_class = class_new(gensym("ofGetPolyline3dCommands"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dCommands_new),
                                                  0, sizeof(t_ofeliaGetPolyline3dCommands),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dCommands_new),
                     gensym("ofGetPolyline3dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommands_bang));
    class_addmethod(ofeliaGetPolyline3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPolyline3dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dBoundingBox object methods
 */
void *ofeliaGetPolyline2dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dBoundingBox *x = reinterpret_cast<t_ofeliaGetPolyline2dBoundingBox*>(pd_new(ofeliaGetPolyline2dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline2dBoundingBox_bang(t_ofeliaGetPolyline2dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPolyline2d::polylineData[pos].boxDimen.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dBoundingBox::objName);
    }
}

void ofeliaGetPolyline2dBoundingBox_set(t_ofeliaGetPolyline2dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dBoundingBox_print(t_ofeliaGetPolyline2dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dBoundingBox_setup()
{
    ofeliaGetPolyline2dBoundingBox_class = class_new(gensym("ofGetPolyline2dBoundingBox"),
                                                     reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dBoundingBox_new),
                                                     0, sizeof(t_ofeliaGetPolyline2dBoundingBox),
                                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dBoundingBox_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline2dBoundingBox_bang));
    class_addmethod(ofeliaGetPolyline2dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dBoundingBox object methods
 */
void *ofeliaGetPolyline3dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dBoundingBox *x = reinterpret_cast<t_ofeliaGetPolyline3dBoundingBox*>(pd_new(ofeliaGetPolyline3dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline3dBoundingBox_bang(t_ofeliaGetPolyline3dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].boxDimen.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dBoundingBox::objName);
    }
}

void ofeliaGetPolyline3dBoundingBox_set(t_ofeliaGetPolyline3dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dBoundingBox_print(t_ofeliaGetPolyline3dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dBoundingBox_setup()
{
    ofeliaGetPolyline3dBoundingBox_class = class_new(gensym("ofGetPolyline3dBoundingBox"),
                                                     reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dBoundingBox_new),
                                                     0, sizeof(t_ofeliaGetPolyline3dBoundingBox),
                                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dBoundingBox_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline3dBoundingBox_bang));
    class_addmethod(ofeliaGetPolyline3dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dBoundingBox_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dCentroid object methods
 */
void *ofeliaGetPolyline2dCentroid_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dCentroid *x = reinterpret_cast<t_ofeliaGetPolyline2dCentroid*>(pd_new(ofeliaGetPolyline2dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline2dCentroid_bang(t_ofeliaGetPolyline2dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPolyline2d::polylineData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPolyline2d::polylineData[pos].centroid.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dCentroid::objName);
    }
}

void ofeliaGetPolyline2dCentroid_set(t_ofeliaGetPolyline2dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dCentroid_print(t_ofeliaGetPolyline2dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dCentroid_setup()
{
    ofeliaGetPolyline2dCentroid_class = class_new(gensym("ofGetPolyline2dCentroid"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dCentroid_new),
                                                  0, sizeof(t_ofeliaGetPolyline2dCentroid),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dCentroid_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline2dCentroid_bang));
    class_addmethod(ofeliaGetPolyline2dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dCentroid object methods
 */
void *ofeliaGetPolyline3dCentroid_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dCentroid *x = reinterpret_cast<t_ofeliaGetPolyline3dCentroid*>(pd_new(ofeliaGetPolyline3dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPolyline3dCentroid_bang(t_ofeliaGetPolyline3dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].centroid.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadPolyline3d::polylineData[pos].centroid.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dCentroid::objName);
    }
}

void ofeliaGetPolyline3dCentroid_set(t_ofeliaGetPolyline3dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dCentroid_print(t_ofeliaGetPolyline3dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dCentroid_setup()
{
    ofeliaGetPolyline3dCentroid_class = class_new(gensym("ofGetPolyline3dCentroid"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dCentroid_new),
                                                  0, sizeof(t_ofeliaGetPolyline3dCentroid),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dCentroid_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline3dCentroid_bang));
    class_addmethod(ofeliaGetPolyline3dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dCentroid_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dArea object methods
 */
void *ofeliaGetPolyline2dArea_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dArea *x = reinterpret_cast<t_ofeliaGetPolyline2dArea*>(pd_new(ofeliaGetPolyline2dArea_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPolyline2dArea_bang(t_ofeliaGetPolyline2dArea *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline2dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPolyline2d::polylineData[pos].area);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dArea::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dArea::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dArea::objName);
    }
}

void ofeliaGetPolyline2dArea_set(t_ofeliaGetPolyline2dArea *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dArea_print(t_ofeliaGetPolyline2dArea *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dArea::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dArea_setup()
{
    ofeliaGetPolyline2dArea_class = class_new(gensym("ofGetPolyline2dArea"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dArea_new),
                                              0, sizeof(t_ofeliaGetPolyline2dArea),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dArea_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline2dArea_bang));
    class_addmethod(ofeliaGetPolyline2dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dArea_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dArea_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dArea_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dArea object methods
 */
void *ofeliaGetPolyline3dArea_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dArea *x = reinterpret_cast<t_ofeliaGetPolyline3dArea*>(pd_new(ofeliaGetPolyline3dArea_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPolyline3dArea_bang(t_ofeliaGetPolyline3dArea *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline3dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPolyline3d::polylineData[pos].area);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dArea::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dArea::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dArea::objName);
    }
}

void ofeliaGetPolyline3dArea_set(t_ofeliaGetPolyline3dArea *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dArea_print(t_ofeliaGetPolyline3dArea *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dArea::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dArea_setup()
{
    ofeliaGetPolyline3dArea_class = class_new(gensym("ofGetPolyline3dArea"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dArea_new),
                                              0, sizeof(t_ofeliaGetPolyline3dArea),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dArea_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline3dArea_bang));
    class_addmethod(ofeliaGetPolyline3dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dArea_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dArea_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dArea_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dArea_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline2dPerimeter object methods
 */
void *ofeliaGetPolyline2dPerimeter_new(t_symbol *s)
{
    t_ofeliaGetPolyline2dPerimeter *x = reinterpret_cast<t_ofeliaGetPolyline2dPerimeter*>(pd_new(ofeliaGetPolyline2dPerimeter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPolyline2dPerimeter_bang(t_ofeliaGetPolyline2dPerimeter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline2d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline2d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline2dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPolyline2d::polylineData[pos].perimeter);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline2dPerimeter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline2dPerimeter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline2dPerimeter::objName);
    }
}

void ofeliaGetPolyline2dPerimeter_set(t_ofeliaGetPolyline2dPerimeter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline2dPerimeter_print(t_ofeliaGetPolyline2dPerimeter *x)
{
    post("\n[%s]", t_ofeliaGetPolyline2dPerimeter::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline2dPerimeter_setup()
{
    ofeliaGetPolyline2dPerimeter_class = class_new(gensym("ofGetPolyline2dPerimeter"),
                                                   reinterpret_cast<t_newmethod>(ofeliaGetPolyline2dPerimeter_new),
                                                   0, sizeof(t_ofeliaGetPolyline2dPerimeter),
                                                   CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline2dPerimeter_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline2dPerimeter_bang));
    class_addmethod(ofeliaGetPolyline2dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dPerimeter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dPerimeter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline2dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline2dPerimeter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPolyline3dPerimeter object methods
 */
void *ofeliaGetPolyline3dPerimeter_new(t_symbol *s)
{
    t_ofeliaGetPolyline3dPerimeter *x = reinterpret_cast<t_ofeliaGetPolyline3dPerimeter*>(pd_new(ofeliaGetPolyline3dPerimeter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPolyline3dPerimeter_bang(t_ofeliaGetPolyline3dPerimeter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPolyline3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPolyline3d::polylineData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPolyline3d::polylineData[pos].shouldUpdateProperties)
                    updatePolyline3dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPolyline3d::polylineData[pos].perimeter);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPolyline3dPerimeter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPolyline3dPerimeter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPolyline3dPerimeter::objName);
    }
}

void ofeliaGetPolyline3dPerimeter_set(t_ofeliaGetPolyline3dPerimeter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPolyline3dPerimeter_print(t_ofeliaGetPolyline3dPerimeter *x)
{
    post("\n[%s]", t_ofeliaGetPolyline3dPerimeter::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPolyline3dPerimeter_setup()
{
    ofeliaGetPolyline3dPerimeter_class = class_new(gensym("ofGetPolyline3dPerimeter"),
                                                   reinterpret_cast<t_newmethod>(ofeliaGetPolyline3dPerimeter_new),
                                                   0, sizeof(t_ofeliaGetPolyline3dPerimeter),
                                                   CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPolyline3dPerimeter_class,
                  reinterpret_cast<t_method>(ofeliaGetPolyline3dPerimeter_bang));
    class_addmethod(ofeliaGetPolyline3dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dPerimeter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dPerimeter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPolyline3dPerimeter_class,
                    reinterpret_cast<t_method>(ofeliaGetPolyline3dPerimeter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaPolyline_setup()
{
    ofeliaLoadPolyline2d_setup();
    ofeliaLoadPolyline3d_setup();
    ofeliaDrawPolyline2d_setup();
    ofeliaDrawPolyline3d_setup();
    ofeliaDoesPolyline2dNameExist_setup();
    ofeliaDoesPolyline3dNameExist_setup();
    ofeliaEditPolyline2dPoint_setup();
    ofeliaEditPolyline3dPoint_setup();
    ofeliaGetPolyline2dPoint_setup();
    ofeliaGetPolyline3dPoint_setup();
    ofeliaGetPolyline2dPoints_setup();
    ofeliaGetPolyline3dPoints_setup();
    ofeliaIsPointInsidePolyline2d_setup();
    ofeliaIsPointInsidePolyline3d_setup();
    ofeliaGetPolyline2dCommand_setup();
    ofeliaGetPolyline3dCommand_setup();
    ofeliaGetPolyline2dCommands_setup();
    ofeliaGetPolyline3dCommands_setup();
    ofeliaGetPolyline2dBoundingBox_setup();
    ofeliaGetPolyline3dBoundingBox_setup();
    ofeliaGetPolyline2dCentroid_setup();
    ofeliaGetPolyline3dCentroid_setup();
    ofeliaGetPolyline2dArea_setup();
    ofeliaGetPolyline3dArea_setup();
    ofeliaGetPolyline2dPerimeter_setup();
    ofeliaGetPolyline3dPerimeter_setup();
}

