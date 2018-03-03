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

#include "ofeliaPath.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadPath2d::objName = "ofLoadPath2d";
unsigned int t_ofeliaLoadPath2d::counter;
vector<t_ofeliaLoadPath2dData> t_ofeliaLoadPath2d::pathData;
vector<t_ofeliaLoadPathSet> t_ofeliaLoadPath2d::pathSets;
bool t_ofeliaLoadPath2d::bInited;
const char *t_ofeliaLoadPath3d::objName = "ofLoadPath3d";
unsigned int t_ofeliaLoadPath3d::counter;
vector<t_ofeliaLoadPath3dData> t_ofeliaLoadPath3d::pathData;
vector<t_ofeliaLoadPathSet> t_ofeliaLoadPath3d::pathSets;
bool t_ofeliaLoadPath3d::bInited;
const char *t_ofeliaDrawPath2d::objName = "ofDrawPath2d";
const char *t_ofeliaDrawPath3d::objName = "ofDrawPath3d";
const char *t_ofeliaDoesPath2dNameExist::objName = "ofDoesPath2dNameExist";
const char *t_ofeliaDoesPath3dNameExist::objName = "ofDoesPath3dNameExist";
const char *t_ofeliaGetPath2dPoint::objName = "ofGetPath2dPoint";
const char *t_ofeliaGetPath3dPoint::objName = "ofGetPath3dPoint";
const char *t_ofeliaGetPath2dPoints::objName = "ofGetPath2dPoints";
const char *t_ofeliaGetPath3dPoints::objName = "ofGetPath3dPoints";
const char *t_ofeliaIsPointInsidePath2d::objName = "ofIsPointInsidePath2d";
const char *t_ofeliaIsPointInsidePath3d::objName = "ofIsPointInsidePath3d";
const char *t_ofeliaGetPath2dCommand::objName = "ofGetPath2dCommand";
const char *t_ofeliaGetPath3dCommand::objName = "ofGetPath3dCommand";
const char *t_ofeliaGetPath2dCommands::objName = "ofGetPath2dCommands";
const char *t_ofeliaGetPath3dCommands::objName = "ofGetPath3dCommands";
const char *t_ofeliaGetPath2dTessellation::objName = "ofGetPath2dTessellation";
const char *t_ofeliaGetPath3dTessellation::objName = "ofGetPath3dTessellation";
const char *t_ofeliaGetPath2dBoundingBox::objName = "ofGetPath2dBoundingBox";
const char *t_ofeliaGetPath3dBoundingBox::objName = "ofGetPath3dBoundingBox";
const char *t_ofeliaGetPath2dCentroid::objName = "ofGetPath2dCentroid";
const char *t_ofeliaGetPath3dCentroid::objName = "ofGetPath3dCentroid";
const char *t_ofeliaGetPath2dArea::objName = "ofGetPath2dArea";
const char *t_ofeliaGetPath3dArea::objName = "ofGetPath3dArea";
const char *t_ofeliaGetPath2dPerimeter::objName = "ofGetPath2dPerimeter";
const char *t_ofeliaGetPath3dPerimeter::objName = "ofGetPath3dPerimeter";

/* ________________________________________________________________________________
 * common methods
 */
void getLoadPathCmdTypeString(LoadPathCmdType type, t_string &name)
{
    switch (type) {
            
        case PATH_LOAD_CMDTYPE_NEWSUBPATH:
            name = "newSubPath";
            break;
        case PATH_LOAD_CMDTYPE_VERTEX:
            name = "vertex";
            break;
        case PATH_LOAD_CMDTYPE_MOVETO:
            name = "moveTo";
            break;
        case PATH_LOAD_CMDTYPE_LINETO:
            name = "lineTo";
            break;
        case PATH_LOAD_CMDTYPE_ARC:
            name = "arc";
            break;
        case PATH_LOAD_CMDTYPE_CURVETO:
            name = "curveTo";
            break;
        case PATH_LOAD_CMDTYPE_BEZIERTO:
            name = "bezierTo";
            break;
        case PATH_LOAD_CMDTYPE_QUADBEZIERTO:
            name = "quadBezierTo";
            break;
        case PATH_LOAD_CMDTYPE_CLOSE:
            name = "close";
            break;
        case PATH_LOAD_CMDTYPE_CIRCLE:
            name = "circle";
            break;
        case PATH_LOAD_CMDTYPE_ELLIPSE:
            name = "ellipse";
            break;
        case PATH_LOAD_CMDTYPE_SECTOR:
            name = "sector";
            break;
        case PATH_LOAD_CMDTYPE_TRIANGLE:
            name = "triangle";
            break;
        case PATH_LOAD_CMDTYPE_EQTRIANGLE:
            name = "eqTriangle";
            break;
        case PATH_LOAD_CMDTYPE_ISOTRIANGLE:
            name = "isoTriangle";
            break;
        case PATH_LOAD_CMDTYPE_QUAD:
            name = "quad";
            break;
        case PATH_LOAD_CMDTYPE_SQUARE:
            name = "square";
            break;
        case PATH_LOAD_CMDTYPE_RECTANGLE:
            name = "rectangle";
            break;
        case PATH_LOAD_CMDTYPE_RECTROUNDED:
            name = "rectRounded";
            break;
        case PATH_LOAD_CMDTYPE_RECTROUNDED4:
            name = "rectRounded4";
            break;
        case PATH_LOAD_CMDTYPE_CROSS:
            name = "cross";
            break;
        case PATH_LOAD_CMDTYPE_HEART:
            name = "heart";
            break;
        case PATH_LOAD_CMDTYPE_MOON:
            name = "moon";
            break;
        case PATH_LOAD_CMDTYPE_REGPOLYGON:
            name = "regPolygon";
            break;
        case PATH_LOAD_CMDTYPE_STAR:
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

void printPolyMode(ofPolyWindingMode polyMode)
{
    t_string mode;
    
    switch (polyMode) {
            
        case OF_POLY_WINDING_ODD:
            mode = "ODD";
            break;
        case OF_POLY_WINDING_NONZERO:
            mode = "NONZERO";
            break;
        case OF_POLY_WINDING_POSITIVE:
            mode = "POSITIVE";
            break;
        case OF_POLY_WINDING_NEGATIVE:
            mode = "NEGATIVE";
            break;
        case OF_POLY_WINDING_ABS_GEQ_TWO:
            mode = "ABS_GEQ_TWO";
            break;
        default:
            break;
    }
    post("polyMode : %s", mode.c_str());
}

void updatePath2dProperties(const int pos)
{
    ofPolyline polylineMerged;
    float perimeterMerged = 0.0f;
    
    for (size_t i=0; i<t_ofeliaLoadPath2d::pathSets[pos].polylines->size(); ++i) {
        
        polylineMerged.addVertices(t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).getVertices());
        perimeterMerged += t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).getPerimeter();
    }
    getBoundingBoxDimen2d(t_ofeliaLoadPath2d::pathData[pos].boxDimen, polylineMerged.getVertices());
    t_ofeliaLoadPath2d::pathData[pos].centroid = polylineMerged.getCentroid2D();
    t_ofeliaLoadPath2d::pathData[pos].area = polylineMerged.getArea();
    t_ofeliaLoadPath2d::pathData[pos].perimeter = perimeterMerged;
    t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties = false;
}

void updatePath3dProperties(const int pos)
{
    ofPolyline polylineMerged;
    float perimeterMerged = 0.0f;
    
    for (size_t i=0; i<t_ofeliaLoadPath3d::pathSets[pos].polylines->size(); ++i) {
        
        polylineMerged.addVertices(t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).getVertices());
        perimeterMerged += t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).getPerimeter();
    }
    getBoundingBoxDimen3d(t_ofeliaLoadPath3d::pathData[pos].boxDimen, polylineMerged.getVertices());
    t_ofeliaLoadPath3d::pathData[pos].centroid = polylineMerged.getCentroid2D();
    t_ofeliaLoadPath3d::pathData[pos].area = polylineMerged.getArea();
    t_ofeliaLoadPath3d::pathData[pos].perimeter = perimeterMerged;
    t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties = false;
}

/* ________________________________________________________________________________
 * ofLoadPath2d object methods
 */
void ofeliaLoadPath2d_clear(t_ofeliaLoadPath2d *x);
void ofeliaLoadPath2d_vecSizeOut(t_ofeliaLoadPath2d *x);

int getPositionByPath2dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadPath2d::pathData), end(t_ofeliaLoadPath2d::pathData),
                            [&objID](const t_ofeliaLoadPath2dData &pathData)
                            {
                                return pathData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadPath2d::pathData.begin());
}

bool setLoadPath2dElemType(const char *name, t_ofeliaLoadPath2dElem &elem)
{
    if (!strcmp(name, "newSubPath"))  {
        
        elem.type = PATH_LOAD_CMDTYPE_NEWSUBPATH;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "vertex")) {
        
        elem.type = PATH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moveTo"))  {
        
        elem.type = PATH_LOAD_CMDTYPE_MOVETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "lineTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_LINETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "arc")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ARC;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "curveTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CURVETO;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "bezierTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_BEZIERTO;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "quadBezierTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_QUADBEZIERTO;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "close")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CLOSE;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = PATH_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "triangle") || !strcmp(name, "tri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_TRIANGLE;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "quad")) {
        
        elem.type = PATH_LOAD_CMDTYPE_QUAD;
        elem.numArgs = 8;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = PATH_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = PATH_LOAD_CMDTYPE_HEART;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = PATH_LOAD_CMDTYPE_MOON;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = PATH_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = PATH_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadPath2d::objName, name);
        return 0;
    }
    return 1;
}

void setPathFromLoadPath2dElems(t_ofeliaLoadPath2d *x,
                                unique_ptr<ofPath> &path,
                                vector<t_ofeliaLoadPath2dElem> &elems)
{
    ofVec2f curPos;
    
    for (size_t i=0; i<elems.size(); ++i) {
        
        switch (elems[i].type) {
                
            case PATH_LOAD_CMDTYPE_NEWSUBPATH:
            {
                path->newSubPath();
                break;
            }
            case PATH_LOAD_CMDTYPE_VERTEX:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->lineTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_MOVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->moveTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_LINETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->lineTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_ARC:
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
            case PATH_LOAD_CMDTYPE_CURVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->curveTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_BEZIERTO:
            {
                curPos.set(elems[i].args[4], elems[i].args[5]);
                path->bezierTo(ofVec2f(elems[i].args[0], elems[i].args[1]),
                               ofVec2f(elems[i].args[2], elems[i].args[3]),
                               curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_QUADBEZIERTO:
            {
                curPos.set(elems[i].args[4], elems[i].args[5]);
                path->quadBezierTo(ofVec2f(elems[i].args[0], elems[i].args[1]),
                                   ofVec2f(elems[i].args[2], elems[i].args[3]),
                                   curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_CLOSE:
            {
                path->close();
                
                if (!path->getOutline().empty())
                    curPos = path->getOutline().back().getVertices()[0];
                else
                    curPos.set(0.0f, 0.0f);
                break;
            }
            case PATH_LOAD_CMDTYPE_CIRCLE:
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
            case PATH_LOAD_CMDTYPE_ELLIPSE:
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
            case PATH_LOAD_CMDTYPE_SECTOR:
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
            case PATH_LOAD_CMDTYPE_TRIANGLE:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->triangle(curPos.x, curPos.y, 0.0f,
                               elems[i].args[2], elems[i].args[3], 0.0f,
                               elems[i].args[4], elems[i].args[5], 0.0f);
                break;
            }
            case PATH_LOAD_CMDTYPE_EQTRIANGLE:
            {
                const float halfSize = elems[i].args[0] * 0.5f;
                const float inRadius = elems[i].args[0] * ofeliaConstants::heightFactor / 3;
                path->triangle(curPos.x, curPos.y-inRadius*2.0f, 0.0f,
                               curPos.x+halfSize, curPos.y+inRadius, 0.0f,
                               curPos.x-halfSize, curPos.y+inRadius, 0.0f);
                break;
            }
            case PATH_LOAD_CMDTYPE_ISOTRIANGLE:
            {
                const float halfWidth = elems[i].args[0] * 0.5f;
                const float halfHeight = elems[i].args[1] * 0.5f;
                path->triangle(curPos.x, curPos.y-halfHeight, 0.0f,
                               curPos.x+halfWidth, curPos.y+halfHeight, 0.0f,
                               curPos.x-halfWidth, curPos.y+halfHeight, 0.0f);
                break;
            }
            case PATH_LOAD_CMDTYPE_QUAD:
            {
                curPos.set(elems[i].args[0], elems[i].args[1]);
                path->moveTo(curPos);
                path->lineTo(ofVec2f(elems[i].args[2], elems[i].args[3]));
                path->lineTo(ofVec2f(elems[i].args[4], elems[i].args[5]));
                path->lineTo(ofVec2f(elems[i].args[6], elems[i].args[7]));
                path->close();
                break;
            }
            case PATH_LOAD_CMDTYPE_SQUARE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER) {
                    
                    const float halfSize = elems[i].args[0] * 0.5f;
                    curPos -= ofVec2f(halfSize, halfSize);
                }
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[0]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTANGLE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[1]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTROUNDED:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, 0.0f,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[2],
                                  elems[i].args[2], elems[i].args[2]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTROUNDED4:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec2f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, 0.0f,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[3],
                                  elems[i].args[4], elems[i].args[5]);
                break;
            }
            case PATH_LOAD_CMDTYPE_CROSS:
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
            case PATH_LOAD_CMDTYPE_HEART:
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
            case PATH_LOAD_CMDTYPE_MOON:
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
            case PATH_LOAD_CMDTYPE_REGPOLYGON:
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
            case PATH_LOAD_CMDTYPE_STAR:
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

bool getLoadPath2dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPath2dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadPath2dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPath2d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPath2d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPath2d::objName);
        return 0;
    }
    return 1;
}

bool getLoadPath2dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadPath2dElem> &elems)
{
    t_string name;
    t_ofeliaLoadPath2dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadPath2dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPath2d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPath2d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPath2dCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadPath2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadPath2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath2d *x = reinterpret_cast<t_ofeliaLoadPath2d*>(pd_new(ofeliaLoadPath2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadPath2dData pathData;
    getVarNameLocalPrefixes(pathData.varName);
    vector<t_ofeliaLoadPath2dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            pathData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(pathData.varName);
            
            if (--argc) {
                
                if (!getLoadPath2dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->rectMode = OF_RECTMODE_CORNER;
    x->polyMode = OF_POLY_WINDING_ODD;
    x->circleRes = x->curveRes = OFELIA_DEFAULT_RESOLUTION;
    x->simplify = 0.0f;
    x->objID = pathData.objID = t_ofeliaLoadPath2d::counter++;
    t_ofeliaLoadPath2d::pathData.push_back(pathData);
    t_ofeliaLoadPath2d::pathSets.push_back({make_unique<ofPath>(),
                                        make_unique<ofVboMesh>(),
                                        make_unique<vector<ofPolyline>>()});
    x->vecSize = 0;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    x->numPoints_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadPath2d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadPath2d_init(t_ofeliaLoadPath2d *x)
{
    if (!t_ofeliaLoadPath2d::bInited && x->bInitGate) {
        
        t_ofeliaLoadPath2d::pathSets.clear();

        for (size_t i=0; i<t_ofeliaLoadPath2d::pathData.size(); ++i)
            t_ofeliaLoadPath2d::pathSets.push_back({make_unique<ofPath>(),
                                                make_unique<ofVboMesh>(),
                                                make_unique<vector<ofPolyline>>()});
        t_ofeliaLoadPath2d::bInited = true;
    }
}

void ofeliaLoadPath2d_update(t_ofeliaLoadPath2d *x)
{
    const int pos = getPositionByPath2dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadPath2d::pathData[pos].elems.size());
            const auto elemsBegin = t_ofeliaLoadPath2d::pathData[pos].elems.begin();
            const t_ofeliaLoadPath2dElem &elem = x->cmdVec[i].elem;
            
            switch (x->cmdVec[i].state) {
                    
                case PATH_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadPath2d::pathData[pos].elems.insert(elemsBegin + fromIndex,
                                                                insertSize, elem);
                    break;
                }
                case PATH_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadPath2d::pathData[pos].elems.erase(elemsBegin + fromIndex,
                                                                   elemsBegin + toIndex);
                    }
                    break;
                }
                case PATH_LOAD_CMD_FILL:
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
        
        t_ofeliaLoadPath2d::pathSets[pos].path->clear();
        t_ofeliaLoadPath2d::pathSets[pos].path->setPolyWindingMode(OF_POLY_WINDING_ODD);
        t_ofeliaLoadPath2d::pathSets[pos].path->setCircleResolution(x->circleRes);
        t_ofeliaLoadPath2d::pathSets[pos].path->setCurveResolution(x->curveRes);
        setPathFromLoadPath2dElems(x, t_ofeliaLoadPath2d::pathSets[pos].path, t_ofeliaLoadPath2d::pathData[pos].elems);
        
        if (x->simplify != 0.0f)
            t_ofeliaLoadPath2d::pathSets[pos].path->simplify(x->simplify);
        t_ofeliaLoadPath2d::pathSets[pos].polylines->operator=(t_ofeliaLoadPath2d::pathSets[pos].path->getOutline());
        t_ofeliaLoadPath2d::pathSets[pos].path->setPolyWindingMode(x->polyMode);
        t_ofeliaLoadPath2d::pathSets[pos].mesh->operator=(t_ofeliaLoadPath2d::pathSets[pos].path->getTessellation());
        
        /* find point indices of the path */
        t_ofeliaLoadPath2d::pathData[pos].pointIndices.clear();
        
        for (size_t i=0; i<t_ofeliaLoadPath2d::pathSets[pos].polylines->size(); ++i) {
            
            const size_t numVertices = t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).getVertices().size();
            
            for (size_t j=0; j<numVertices; ++j)
                t_ofeliaLoadPath2d::pathData[pos].pointIndices.push_back(make_pair(i, j));
        }
        t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties = true;
        
        /* output number of points and current size of the path */
        x->numPoints = static_cast<int>(t_ofeliaLoadPath2d::pathData[pos].pointIndices.size());
        x->vecSize = static_cast<int>(t_ofeliaLoadPath2d::pathData[pos].elems.size());
        ofeliaLoadPath2d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadPath2d_exit(t_ofeliaLoadPath2d *x)
{
    if (t_ofeliaLoadPath2d::bInited)
        t_ofeliaLoadPath2d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadPath2d_vecSizeOut(t_ofeliaLoadPath2d *x)
{
    outlet_float(x->numPoints_out, static_cast<t_float>(x->numPoints));
    outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadPath2d_name(t_ofeliaLoadPath2d *x, t_symbol *s)
{
    const int pos = getPositionByPath2dObjID(x->objID);
    t_ofeliaLoadPath2d::pathData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadPath2d::pathData[pos].varName);
}

void ofeliaLoadPath2d_rectMode(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaLoadPath2d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPath2d_polyMode(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getPolyModeFromArgs(argc, argv, x->polyMode, t_ofeliaLoadPath2d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPath2d_circleRes(t_ofeliaLoadPath2d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: circleRes out of range", t_ofeliaLoadPath2d::objName);
        return;
    }
    x->circleRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPath2d_curveRes(t_ofeliaLoadPath2d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: curveRes out of range", t_ofeliaLoadPath2d::objName);
        return;
    }
    x->curveRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPath2d_simplify(t_ofeliaLoadPath2d *x, t_floatarg f)
{
    const float simplify = f;
    
    if (simplify < 0.0f) {
        
        error("%s: simplify out of range", t_ofeliaLoadPath2d::objName);
        return;
    }
    x->simplify = simplify;
    x->shouldOutlet = true;
}

void ofeliaLoadPath2d_load(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath2dElem> elems;
    
    if (getLoadPath2dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadPath2d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath2d_add(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath2dCmdData cmd;
    
    if (getLoadPath2dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = PATH_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPath2d_append(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath2dElem> elems;
    
    if (getLoadPath2dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath2d_prepend(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath2dElem> elems;
    
    if (getLoadPath2dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath2d_insert(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath2dCmdData cmd;
    
    if (getLoadPath2dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;

        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath2d_fill(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath2dCmdData cmd;
    
    if (getLoadPath2dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = PATH_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath2d_erase(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath2dCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = PATH_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPath2d_clear(t_ofeliaLoadPath2d *x)
{
    t_ofeliaLoadPath2dCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = PATH_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadPath2d_set(t_ofeliaLoadPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadPath2d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadPath2d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath2d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPath2d::objName);
    }
}

void ofeliaLoadPath2d_print(t_ofeliaLoadPath2d *x)
{
    post("\n[%s]", t_ofeliaLoadPath2d::objName);
    const int pos = getPositionByPath2dObjID(x->objID);
    post("name : %s", t_ofeliaLoadPath2d::pathData[pos].varName.name.c_str());
    printRectMode(x->rectMode);
    printPolyMode(x->polyMode);
    post("circleRes : %d", x->circleRes);
    post("curveRes : %d", x->curveRes);
    post("simplify : %g", x->simplify);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadPath2d::pathData[pos].elems.size(); ++i) {
        
        stringstream ss;
        const char space = ' ';
        t_string name;
        getLoadPathCmdTypeString(t_ofeliaLoadPath2d::pathData[pos].elems[i].type, name);
        ss << name;
        
        for (size_t j=0; j<t_ofeliaLoadPath2d::pathData[pos].elems[i].numArgs; ++j)
            ss << space << t_ofeliaLoadPath2d::pathData[pos].elems[i].args[j];
        post(" %s", ss.str().c_str());
    }
}

void ofeliaLoadPath2d_free(t_ofeliaLoadPath2d *x)
{
    const int pos = getPositionByPath2dObjID(x->objID);
    t_ofeliaLoadPath2d::pathData.erase(t_ofeliaLoadPath2d::pathData.begin() + pos);
    t_ofeliaLoadPath2d::pathSets.erase(t_ofeliaLoadPath2d::pathSets.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->vecSize_out);
    outlet_free(x->numPoints_out);
}

void ofeliaLoadPath2d_setup()
{
    ofeliaLoadPath2d_class = class_new(gensym("ofLoadPath2d"),
                                       reinterpret_cast<t_newmethod>(ofeliaLoadPath2d_new),
                                       reinterpret_cast<t_method>(ofeliaLoadPath2d_free),
                                       sizeof(t_ofeliaLoadPath2d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_rectMode),
                    gensym("rectMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_polyMode),
                    gensym("polyMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_circleRes),
                    gensym("circleRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_curveRes),
                    gensym("curveRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_simplify),
                    gensym("simplify"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath2d_class, reinterpret_cast<t_method>(ofeliaLoadPath2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLoadPath3d object methods
 */
void ofeliaLoadPath3d_clear(t_ofeliaLoadPath3d *x);
void ofeliaLoadPath3d_vecSizeOut(t_ofeliaLoadPath3d *x);

int getPositionByPath3dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadPath3d::pathData), end(t_ofeliaLoadPath3d::pathData),
                            [&objID](const t_ofeliaLoadPath3dData &pathData)
                            {
                                return pathData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadPath3d::pathData.begin());
}

bool setLoadPath3dElemType(const char *name, t_ofeliaLoadPath3dElem &elem)
{
    if (!strcmp(name, "newSubPath"))  {
        
        elem.type = PATH_LOAD_CMDTYPE_NEWSUBPATH;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "vertex")) {
        
        elem.type = PATH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "moveTo"))  {
        
        elem.type = PATH_LOAD_CMDTYPE_MOVETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "lineTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_LINETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "arc")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ARC;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "curveTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CURVETO;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "bezierTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_BEZIERTO;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "quadBezierTo")) {
        
        elem.type = PATH_LOAD_CMDTYPE_QUADBEZIERTO;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "close")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CLOSE;
        elem.numArgs = 0;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = PATH_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "triangle") || !strcmp(name, "tri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_TRIANGLE;
        elem.numArgs = 9;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = PATH_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "quad")) {
        
        elem.type = PATH_LOAD_CMDTYPE_QUAD;
        elem.numArgs = 12;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = PATH_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = PATH_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = PATH_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = PATH_LOAD_CMDTYPE_HEART;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = PATH_LOAD_CMDTYPE_MOON;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = PATH_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = PATH_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadPath3d::objName, name);
        return 0;
    }
    return 1;
}

void setPathFromPath3dElems(t_ofeliaLoadPath3d *x,
                            unique_ptr<ofPath> &path,
                            vector<t_ofeliaLoadPath3dElem> &elems)
{
    ofVec3f curPos;
    
    for (size_t i=0; i<elems.size(); ++i) {
        
        switch (elems[i].type) {
                
            case PATH_LOAD_CMDTYPE_NEWSUBPATH:
            {
                path->newSubPath();
                break;
            }
            case PATH_LOAD_CMDTYPE_VERTEX:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->lineTo(ofVec3f(curPos));
                break;
            }
            case PATH_LOAD_CMDTYPE_MOVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->moveTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_LINETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->lineTo(ofVec3f(curPos));
                break;
            }
            case PATH_LOAD_CMDTYPE_ARC:
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
            case PATH_LOAD_CMDTYPE_CURVETO:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->curveTo(curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_BEZIERTO:
            {
                curPos.set(elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                path->bezierTo(ofVec3f(elems[i].args[0], elems[i].args[1], elems[i].args[2]),
                               ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]),
                               curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_QUADBEZIERTO:
            {
                curPos.set(elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                path->quadBezierTo(ofVec3f(elems[i].args[0], elems[i].args[1], elems[i].args[2]),
                                   ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]),
                                   curPos);
                break;
            }
            case PATH_LOAD_CMDTYPE_CLOSE:
            {
                path->close();
                
                if (!path->getOutline().empty())
                    curPos = path->getOutline().back().getVertices()[0];
                else
                    curPos.set(0.0f, 0.0f, 0.0f);
                break;
            }
            case PATH_LOAD_CMDTYPE_CIRCLE:
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
            case PATH_LOAD_CMDTYPE_ELLIPSE:
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
            case PATH_LOAD_CMDTYPE_SECTOR:
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
            case PATH_LOAD_CMDTYPE_TRIANGLE:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->triangle(curPos.x, curPos.y, curPos.z,
                               elems[i].args[3], elems[i].args[4], elems[i].args[5],
                               elems[i].args[6], elems[i].args[7], elems[i].args[8]);
                break;
            }
            case PATH_LOAD_CMDTYPE_EQTRIANGLE:
            {
                const float halfSize = elems[i].args[0] * 0.5f;
                const float inRadius = elems[i].args[0] * ofeliaConstants::heightFactor / 3;
                path->triangle(curPos.x, curPos.y-inRadius*2.0f, curPos.z,
                               curPos.x+halfSize, curPos.y+inRadius, curPos.z,
                               curPos.x-halfSize, curPos.y+inRadius, curPos.z);
                break;
            }
            case PATH_LOAD_CMDTYPE_ISOTRIANGLE:
            {
                const float halfWidth = elems[i].args[0] * 0.5f;
                const float halfHeight = elems[i].args[1] * 0.5f;
                path->triangle(curPos.x, curPos.y-halfHeight, curPos.z,
                               curPos.x+halfWidth, curPos.y+halfHeight, curPos.z,
                               curPos.x-halfWidth, curPos.y+halfHeight, curPos.z);
                break;
            }
            case PATH_LOAD_CMDTYPE_QUAD:
            {
                curPos.set(elems[i].args[0], elems[i].args[1], elems[i].args[2]);
                path->moveTo(curPos);
                path->lineTo(ofVec3f(elems[i].args[3], elems[i].args[4], elems[i].args[5]));
                path->lineTo(ofVec3f(elems[i].args[6], elems[i].args[7], elems[i].args[8]));
                path->lineTo(ofVec3f(elems[i].args[9], elems[i].args[10], elems[i].args[11]));
                path->close();
                break;
            }
            case PATH_LOAD_CMDTYPE_SQUARE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER) {
                    
                    const float halfSize = elems[i].args[0] * 0.5f;
                    curPos -= ofVec3f(halfSize, halfSize);
                }
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[0]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTANGLE:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectangle(curPos,
                                elems[i].args[0], elems[i].args[1]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTROUNDED:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, curPos.z,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[2],
                                  elems[i].args[2], elems[i].args[2]);
                break;
            }
            case PATH_LOAD_CMDTYPE_RECTROUNDED4:
            {
                if (x->rectMode == OF_RECTMODE_CENTER)
                    curPos -= ofVec3f(elems[i].args[0] * 0.5f, elems[i].args[1] * 0.5f);
                path->rectRounded(curPos.x, curPos.y, curPos.z,
                                  elems[i].args[0], elems[i].args[1],
                                  elems[i].args[2], elems[i].args[3],
                                  elems[i].args[4], elems[i].args[5]);
                break;
            }
            case PATH_LOAD_CMDTYPE_CROSS:
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
            case PATH_LOAD_CMDTYPE_HEART:
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
            case PATH_LOAD_CMDTYPE_MOON:
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
            case PATH_LOAD_CMDTYPE_REGPOLYGON:
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
            case PATH_LOAD_CMDTYPE_STAR:
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

bool getLoadPath3dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPath3dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadPath3dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPath3d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadPath3d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPath3d::objName);
        return 0;
    }
    return 1;
}

bool getLoadPath3dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadPath3dElem> &elems)
{
    t_string name;
    t_ofeliaLoadPath3dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadPath3dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPath3d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadPath3d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadPath3dCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadPath3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadPath3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath3d *x = reinterpret_cast<t_ofeliaLoadPath3d*>(pd_new(ofeliaLoadPath3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadPath3dData pathData;
    getVarNameLocalPrefixes(pathData.varName);
    vector<t_ofeliaLoadPath3dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            pathData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(pathData.varName);
            
            if (--argc) {
                
                if (!getLoadPath3dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->rectMode = OF_RECTMODE_CORNER;
    x->polyMode = OF_POLY_WINDING_ODD;
    x->circleRes = x->curveRes = OFELIA_DEFAULT_RESOLUTION;
    x->simplify = 0.0f;
    x->objID = pathData.objID = t_ofeliaLoadPath3d::counter++;
    t_ofeliaLoadPath3d::pathData.push_back(pathData);
    t_ofeliaLoadPath3d::pathSets.push_back({make_unique<ofPath>(),
                                        make_unique<ofVboMesh>(),
                                        make_unique<vector<ofPolyline>>()});
    x->vecSize = 0;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    x->numPoints_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadPath3d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadPath3d_init(t_ofeliaLoadPath3d *x)
{
    if (!t_ofeliaLoadPath3d::bInited && x->bInitGate) {
        
        t_ofeliaLoadPath3d::pathSets.clear();
        
        for (size_t i=0; i<t_ofeliaLoadPath3d::pathData.size(); ++i)
            t_ofeliaLoadPath3d::pathSets.push_back({make_unique<ofPath>(),
                make_unique<ofVboMesh>(),
                make_unique<vector<ofPolyline>>()});
        t_ofeliaLoadPath3d::bInited = true;
    }
}

void ofeliaLoadPath3d_update(t_ofeliaLoadPath3d *x)
{
    const int pos = getPositionByPath3dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadPath3d::pathData[pos].elems.size());
            const auto elemsBegin = t_ofeliaLoadPath3d::pathData[pos].elems.begin();
            const t_ofeliaLoadPath3dElem &elem = x->cmdVec[i].elem;
            
            switch (x->cmdVec[i].state) {
                    
                case PATH_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadPath3d::pathData[pos].elems.insert(elemsBegin + fromIndex,
                                                                insertSize, elem);
                    break;
                }
                case PATH_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadPath3d::pathData[pos].elems.erase(elemsBegin + fromIndex,
                                                                   elemsBegin + toIndex);
                    }
                    break;
                }
                case PATH_LOAD_CMD_FILL:
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
        
        t_ofeliaLoadPath3d::pathSets[pos].path->clear();
        t_ofeliaLoadPath3d::pathSets[pos].path->setPolyWindingMode(OF_POLY_WINDING_ODD);
        t_ofeliaLoadPath3d::pathSets[pos].path->setCircleResolution(x->circleRes);
        t_ofeliaLoadPath3d::pathSets[pos].path->setCurveResolution(x->curveRes);
        setPathFromPath3dElems(x, t_ofeliaLoadPath3d::pathSets[pos].path, t_ofeliaLoadPath3d::pathData[pos].elems);
        
        if (x->simplify != 0.0f)
            t_ofeliaLoadPath3d::pathSets[pos].path->simplify(x->simplify);
        t_ofeliaLoadPath3d::pathSets[pos].polylines->operator=(t_ofeliaLoadPath3d::pathSets[pos].path->getOutline());
        t_ofeliaLoadPath3d::pathSets[pos].path->setPolyWindingMode(x->polyMode);
        t_ofeliaLoadPath3d::pathSets[pos].mesh->operator=(t_ofeliaLoadPath3d::pathSets[pos].path->getTessellation());
        
        /* find point indices of the path */
        t_ofeliaLoadPath3d::pathData[pos].pointIndices.clear();
        
        for (size_t i=0; i<t_ofeliaLoadPath3d::pathSets[pos].polylines->size(); ++i) {
            
            const size_t numVertices = t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).getVertices().size();
            
            for (size_t j=0; j<numVertices; ++j)
                t_ofeliaLoadPath3d::pathData[pos].pointIndices.push_back(make_pair(i, j));
        }
        t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties = true;
        
        /* output number of points and current size of the path */
        x->numPoints = static_cast<int>(t_ofeliaLoadPath3d::pathData[pos].pointIndices.size());
        x->vecSize = static_cast<int>(t_ofeliaLoadPath3d::pathData[pos].elems.size());
        ofeliaLoadPath3d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadPath3d_exit(t_ofeliaLoadPath3d *x)
{
    if (t_ofeliaLoadPath3d::bInited)
        t_ofeliaLoadPath3d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadPath3d_vecSizeOut(t_ofeliaLoadPath3d *x)
{
    outlet_float(x->numPoints_out, static_cast<t_float>(x->numPoints));
    outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadPath3d_name(t_ofeliaLoadPath3d *x, t_symbol *s)
{
    const int pos = getPositionByPath3dObjID(x->objID);
    t_ofeliaLoadPath3d::pathData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadPath3d::pathData[pos].varName);
}

void ofeliaLoadPath3d_rectMode(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaLoadPath3d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPath3d_polyMode(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getPolyModeFromArgs(argc, argv, x->polyMode, t_ofeliaLoadPath3d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadPath3d_circleRes(t_ofeliaLoadPath3d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: circleRes out of range", t_ofeliaLoadPath3d::objName);
        return;
    }
    x->circleRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPath3d_curveRes(t_ofeliaLoadPath3d *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res < 1) {
        
        error("%s: curveRes out of range", t_ofeliaLoadPath3d::objName);
        return;
    }
    x->curveRes = res;
    x->shouldOutlet = true;
}

void ofeliaLoadPath3d_simplify(t_ofeliaLoadPath3d *x, t_floatarg f)
{
    const float simplify = f;
    
    if (simplify < 0.0f) {
        
        error("%s: simplify out of range", t_ofeliaLoadPath3d::objName);
        return;
    }
    x->simplify = simplify;
    x->shouldOutlet = true;
}

void ofeliaLoadPath3d_load(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath3dElem> elems;
    
    if (getLoadPath3dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadPath3d_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath3d_add(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath3dCmdData cmd;
    
    if (getLoadPath3dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = PATH_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPath3d_append(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath3dElem> elems;
    
    if (getLoadPath3dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath3d_prepend(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadPath3dElem> elems;
    
    if (getLoadPath3dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadPath3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath3d_insert(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath3dCmdData cmd;
    
    if (getLoadPath3dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = PATH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath3d_fill(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath3dCmdData cmd;
    
    if (getLoadPath3dElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = PATH_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadPath3d_erase(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadPath3dCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = PATH_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadPath3d_clear(t_ofeliaLoadPath3d *x)
{
    t_ofeliaLoadPath3dCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = PATH_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadPath3d_set(t_ofeliaLoadPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadPath3d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadPath3d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadPath3d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadPath3d::objName);
    }
}

void ofeliaLoadPath3d_print(t_ofeliaLoadPath3d *x)
{
    post("\n[%s]", t_ofeliaLoadPath3d::objName);
    const int pos = getPositionByPath3dObjID(x->objID);
    post("name : %s", t_ofeliaLoadPath3d::pathData[pos].varName.name.c_str());
    printRectMode(x->rectMode);
    printPolyMode(x->polyMode);
    post("circleRes : %d", x->circleRes);
    post("curveRes : %d", x->curveRes);
    post("simplify : %g", x->simplify);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadPath3d::pathData[pos].elems.size(); ++i) {
        
        stringstream ss;
        const char space = ' ';
        t_string name;
        getLoadPathCmdTypeString(t_ofeliaLoadPath3d::pathData[pos].elems[i].type, name);
        ss << name;
        
        for (size_t j=0; j<t_ofeliaLoadPath3d::pathData[pos].elems[i].numArgs; ++j)
            ss << space << t_ofeliaLoadPath3d::pathData[pos].elems[i].args[j];
        post(" %s", ss.str().c_str());
    }
}

void ofeliaLoadPath3d_free(t_ofeliaLoadPath3d *x)
{
    const int pos = getPositionByPath3dObjID(x->objID);
    t_ofeliaLoadPath3d::pathData.erase(t_ofeliaLoadPath3d::pathData.begin() + pos);
    t_ofeliaLoadPath3d::pathSets.erase(t_ofeliaLoadPath3d::pathSets.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->vecSize_out);
    outlet_free(x->numPoints_out);
}

void ofeliaLoadPath3d_setup()
{
    ofeliaLoadPath3d_class = class_new(gensym("ofLoadPath3d"),
                                       reinterpret_cast<t_newmethod>(ofeliaLoadPath3d_new),
                                       reinterpret_cast<t_method>(ofeliaLoadPath3d_free),
                                       sizeof(t_ofeliaLoadPath3d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_rectMode),
                    gensym("rectMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_polyMode),
                    gensym("polyMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_circleRes),
                    gensym("circleRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_curveRes),
                    gensym("curveRes"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_simplify),
                    gensym("simplify"), A_FLOAT, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadPath3d_class, reinterpret_cast<t_method>(ofeliaLoadPath3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByPath2dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadPath2d::pathData), end(t_ofeliaLoadPath2d::pathData),
                      [&name] (const t_ofeliaLoadPath2dData &pathData) {
                          return (pathData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadPath2d::pathData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadPath2d::pathData));
}

int getPositionByPath3dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadPath3d::pathData), end(t_ofeliaLoadPath3d::pathData),
                      [&name] (const t_ofeliaLoadPath3dData &pathData) {
                          return (pathData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadPath3d::pathData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadPath3d::pathData));
}

/* ________________________________________________________________________________
 * ofDrawPath2d object methods
 */
void *ofeliaDrawPath2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawPath2d *x = reinterpret_cast<t_ofeliaDrawPath2d*>(pd_new(ofeliaDrawPath2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPath2d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawPath2d_bang(t_ofeliaDrawPath2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByPath2dName(name);
            
            if (pos != -1) {
                
                if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                    
                    const bool bFill = ofGetStyle().bFill;
                    
                    if (!x->elem.width && !x->elem.height) {
                        
                        if (!bFill)
                            for (size_t i=0; i<t_ofeliaLoadPath2d::pathSets[pos].polylines->size(); ++i)
                                t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).draw();
                        else
                            t_ofeliaLoadPath2d::pathSets[pos].mesh->draw();
                    }
                    else {
                        
                        if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                            updatePath2dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadPath2d::pathData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadPath2d::pathData[pos].boxDimen.y;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, 1.0f);
                        
                        if (!bFill)
                            for (size_t i=0; i<t_ofeliaLoadPath2d::pathSets[pos].polylines->size(); ++i)
                                t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).draw();
                        else
                            t_ofeliaLoadPath2d::pathSets[pos].mesh->draw();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawPath2d::objName);
    }
}

void ofeliaDrawPath2d_name(t_ofeliaDrawPath2d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawPath2d_width(t_ofeliaDrawPath2d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawPath2d_height(t_ofeliaDrawPath2d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawPath2d_set(t_ofeliaDrawPath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPath2d::objName);
}

void ofeliaDrawPath2d_print(t_ofeliaDrawPath2d *x)
{
    post("\n[%s]", t_ofeliaDrawPath2d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height;
    
    if (!x->elem.width && !x->elem.height) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByPath2dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                    updatePath2dProperties(pos);
                width = t_ofeliaLoadPath2d::pathData[pos].boxDimen.x;
                height = t_ofeliaLoadPath2d::pathData[pos].boxDimen.y;
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

void ofeliaDrawPath2d_setup()
{
    ofeliaDrawPath2d_class = class_new(gensym("ofDrawPath2d"),
                                       reinterpret_cast<t_newmethod>(ofeliaDrawPath2d_new),
                                       0, sizeof(t_ofeliaDrawPath2d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_bang));
    class_addmethod(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawPath2d_class, reinterpret_cast<t_method>(ofeliaDrawPath2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawPath3d object methods
 */
void *ofeliaDrawPath3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawPath3d *x = reinterpret_cast<t_ofeliaDrawPath3d*>(pd_new(ofeliaDrawPath3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPath3d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("depth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawPath3d_bang(t_ofeliaDrawPath3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByPath3dName(name);
            
            if (pos != -1) {
                
                if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                    
                    const bool bFill = ofGetStyle().bFill;
                    
                    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
                        
                        if (!bFill)
                            for (size_t i=0; i<t_ofeliaLoadPath3d::pathSets[pos].polylines->size(); ++i)
                                t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).draw();
                        else
                            t_ofeliaLoadPath3d::pathSets[pos].mesh->draw();
                    }
                    else {
                        
                        if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                            updatePath3dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadPath3d::pathData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadPath3d::pathData[pos].boxDimen.y;
                        const float depthScale = x->elem.depth / t_ofeliaLoadPath3d::pathData[pos].boxDimen.z;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, depthScale);
                        
                        if (!bFill)
                            for (size_t i=0; i<t_ofeliaLoadPath3d::pathSets[pos].polylines->size(); ++i)
                                t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).draw();
                        else
                            t_ofeliaLoadPath3d::pathSets[pos].mesh->draw();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawPath3d::objName);
    }
}

void ofeliaDrawPath3d_name(t_ofeliaDrawPath3d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawPath3d_width(t_ofeliaDrawPath3d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawPath3d_height(t_ofeliaDrawPath3d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawPath3d_depth(t_ofeliaDrawPath3d *x, t_floatarg f)
{
    x->elem.depth = f;
}

void ofeliaDrawPath3d_set(t_ofeliaDrawPath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawPath3d::objName);
}

void ofeliaDrawPath3d_print(t_ofeliaDrawPath3d *x)
{
    post("\n[%s]", t_ofeliaDrawPath3d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height, depth;
    
    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByPath3dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                    updatePath3dProperties(pos);
                width = t_ofeliaLoadPath3d::pathData[pos].boxDimen.x;
                height = t_ofeliaLoadPath3d::pathData[pos].boxDimen.y;
                depth = t_ofeliaLoadPath3d::pathData[pos].boxDimen.z;
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

void ofeliaDrawPath3d_setup()
{
    ofeliaDrawPath3d_class = class_new(gensym("ofDrawPath3d"),
                                       reinterpret_cast<t_newmethod>(ofeliaDrawPath3d_new),
                                       0, sizeof(t_ofeliaDrawPath3d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_bang));
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_depth),
                    gensym("depth"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawPath3d_class, reinterpret_cast<t_method>(ofeliaDrawPath3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesPath2dNameExist object methods
 */
void *ofeliaDoesPath2dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesPath2dNameExist *x = reinterpret_cast<t_ofeliaDoesPath2dNameExist*>(pd_new(ofeliaDoesPath2dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesPath2dNameExist_bang(t_ofeliaDoesPath2dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByPath2dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesPath2dNameExist::objName);
    }
}

void ofeliaDoesPath2dNameExist_set(t_ofeliaDoesPath2dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesPath2dNameExist_print(t_ofeliaDoesPath2dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesPath2dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesPath2dNameExist_setup()
{
    ofeliaDoesPath2dNameExist_class = class_new(gensym("ofDoesPath2dNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesPath2dNameExist_new),
                                               0, sizeof(t_ofeliaDoesPath2dNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesPath2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath2dNameExist_bang));
    class_addmethod(ofeliaDoesPath2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath2dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPath2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath2dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPath2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath2dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesPath3dNameExist object methods
 */
void *ofeliaDoesPath3dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesPath3dNameExist *x = reinterpret_cast<t_ofeliaDoesPath3dNameExist*>(pd_new(ofeliaDoesPath3dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesPath3dNameExist_bang(t_ofeliaDoesPath3dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByPath3dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesPath3dNameExist::objName);
    }
}

void ofeliaDoesPath3dNameExist_set(t_ofeliaDoesPath3dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesPath3dNameExist_print(t_ofeliaDoesPath3dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesPath3dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesPath3dNameExist_setup()
{
    ofeliaDoesPath3dNameExist_class = class_new(gensym("ofDoesPath3dNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesPath3dNameExist_new),
                                               0, sizeof(t_ofeliaDoesPath3dNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesPath3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath3dNameExist_bang));
    class_addmethod(ofeliaDoesPath3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath3dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPath3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath3dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesPath3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesPath3dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dPoint object methods
 */
void *ofeliaGetPath2dPoint_new(t_symbol *s)
{
    t_ofeliaGetPath2dPoint *x = reinterpret_cast<t_ofeliaGetPath2dPoint*>(pd_new(ofeliaGetPath2dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath2dPoint_bang(t_ofeliaGetPath2dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPath2d::pathData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                const ofVec2f &point = t_ofeliaLoadPath2d::pathSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = point.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = point.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dPoint::objName);
    }
}

void ofeliaGetPath2dPoint_float(t_ofeliaGetPath2dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPath2dPoint_set(t_ofeliaGetPath2dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPath2dPoint_print(t_ofeliaGetPath2dPoint *x)
{
    post("\n[%s]", t_ofeliaGetPath2dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPath2dPoint_setup()
{
    ofeliaGetPath2dPoint_class = class_new(gensym("ofGetPath2dPoint"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetPath2dPoint_new),
                                           0, sizeof(t_ofeliaGetPath2dPoint),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoint_bang));
    class_addfloat(ofeliaGetPath2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoint_float));
    class_addmethod(ofeliaGetPath2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dPoint object methods
 */
void *ofeliaGetPath3dPoint_new(t_symbol *s)
{
    t_ofeliaGetPath3dPoint *x = reinterpret_cast<t_ofeliaGetPath3dPoint*>(pd_new(ofeliaGetPath3dPoint_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath3dPoint_bang(t_ofeliaGetPath3dPoint *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPath3d::pathData[pos].pointIndices;
                const int index = min(x->varName.index, static_cast<int>(pointIndices.size()-1));
                const ofVec3f &point = t_ofeliaLoadPath3d::pathSets[pos].polylines->at(pointIndices[index].first).getVertices()[pointIndices[index].second];
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dPoint::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dPoint::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dPoint::objName);
    }
}

void ofeliaGetPath3dPoint_float(t_ofeliaGetPath3dPoint *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPath3dPoint_set(t_ofeliaGetPath3dPoint *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPath3dPoint_print(t_ofeliaGetPath3dPoint *x)
{
    post("\n[%s]", t_ofeliaGetPath3dPoint::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPath3dPoint_setup()
{
    ofeliaGetPath3dPoint_class = class_new(gensym("ofGetPath3dPoint"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetPath3dPoint_new),
                                           0, sizeof(t_ofeliaGetPath3dPoint),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoint_bang));
    class_addfloat(ofeliaGetPath3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoint_float));
    class_addmethod(ofeliaGetPath3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoint_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoint_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPoint_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoint_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dPoints object methods
 */
void *ofeliaGetPath2dPoints_new(t_symbol *s)
{
    t_ofeliaGetPath2dPoints *x = reinterpret_cast<t_ofeliaGetPath2dPoints*>(pd_new(ofeliaGetPath2dPoints_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath2dPoints_bang(t_ofeliaGetPath2dPoints *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPath2d::pathData[pos].pointIndices;
                const int ac = static_cast<int>(pointIndices.size()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const int idx = i/2;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = t_ofeliaLoadPath2d::pathSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = t_ofeliaLoadPath2d::pathSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dPoints::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dPoints::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dPoints::objName);
    }
}

void ofeliaGetPath2dPoints_set(t_ofeliaGetPath2dPoints *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dPoints_print(t_ofeliaGetPath2dPoints *x)
{
    post("\n[%s]", t_ofeliaGetPath2dPoints::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dPoints_setup()
{
    ofeliaGetPath2dPoints_class = class_new(gensym("ofGetPath2dPoints"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetPath2dPoints_new),
                                            0, sizeof(t_ofeliaGetPath2dPoints),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoints_bang));
    class_addmethod(ofeliaGetPath2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoints_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoints_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath2dPoints_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dPoints object methods
 */
void *ofeliaGetPath3dPoints_new(t_symbol *s)
{
    t_ofeliaGetPath3dPoints *x = reinterpret_cast<t_ofeliaGetPath3dPoints*>(pd_new(ofeliaGetPath3dPoints_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath3dPoints_bang(t_ofeliaGetPath3dPoints *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                const vector<pair<size_t, size_t>> &pointIndices = t_ofeliaLoadPath3d::pathData[pos].pointIndices;
                const int ac = static_cast<int>(pointIndices.size()*3);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=3) {
                    
                    const int idx = i/3;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = t_ofeliaLoadPath3d::pathSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = t_ofeliaLoadPath3d::pathSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].y;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = t_ofeliaLoadPath3d::pathSets[pos].polylines->at(pointIndices[idx].first).getVertices()[pointIndices[idx].second].z;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dPoints::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dPoints::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dPoints::objName);
    }
}

void ofeliaGetPath3dPoints_set(t_ofeliaGetPath3dPoints *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dPoints_print(t_ofeliaGetPath3dPoints *x)
{
    post("\n[%s]", t_ofeliaGetPath3dPoints::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dPoints_setup()
{
    ofeliaGetPath3dPoints_class = class_new(gensym("ofGetPath3dPoints"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetPath3dPoints_new),
                                            0, sizeof(t_ofeliaGetPath3dPoints),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoints_bang));
    class_addmethod(ofeliaGetPath3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoints_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoints_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPoints_class, reinterpret_cast<t_method>(ofeliaGetPath3dPoints_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsPointInsidePath2d object methods
 */
void *ofeliaIsPointInsidePath2d_new(t_symbol *s)
{
    t_ofeliaIsPointInsidePath2d *x = reinterpret_cast<t_ofeliaIsPointInsidePath2d*>(pd_new(ofeliaIsPointInsidePath2d_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsPointInsidePath2d_list(t_ofeliaIsPointInsidePath2d *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float posX = argv[0].a_w.w_float;
                        const float posY = argv[1].a_w.w_float;
                        
                        bool inside = false;
                        
                        for (size_t i=0; i<t_ofeliaLoadPath2d::pathSets[pos].polylines->size(); ++i) {
                            
                            if (t_ofeliaLoadPath2d::pathSets[pos].polylines->at(i).inside(posX, posY)) {

                                inside = true;
                                break;
                            }
                        }
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsPointInsidePath2d::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsPointInsidePath2d::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsPointInsidePath2d::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsPointInsidePath2d::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsPointInsidePath2d::objName);
    }
}

void ofeliaIsPointInsidePath2d_set(t_ofeliaIsPointInsidePath2d *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaIsPointInsidePath2d_print(t_ofeliaIsPointInsidePath2d *x)
{
    post("\n[%s]", t_ofeliaIsPointInsidePath2d::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaIsPointInsidePath2d_setup()
{
    ofeliaIsPointInsidePath2d_class = class_new(gensym("ofIsPointInsidePath2d"),
                                            reinterpret_cast<t_newmethod>(ofeliaIsPointInsidePath2d_new),
                                            0, sizeof(t_ofeliaIsPointInsidePath2d),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addlist(ofeliaIsPointInsidePath2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath2d_list));
    class_addmethod(ofeliaIsPointInsidePath2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath2d_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePath2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath2d_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePath2d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsPointInsidePath3d object methods
 */
void *ofeliaIsPointInsidePath3d_new(t_symbol *s)
{
    t_ofeliaIsPointInsidePath3d *x = reinterpret_cast<t_ofeliaIsPointInsidePath3d*>(pd_new(ofeliaIsPointInsidePath3d_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsPointInsidePath3d_list(t_ofeliaIsPointInsidePath3d *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float posX = argv[0].a_w.w_float;
                        const float posY = argv[1].a_w.w_float;
                        
                        bool inside = false;
                        
                        for (size_t i=0; i<t_ofeliaLoadPath3d::pathSets[pos].polylines->size(); ++i) {
                            
                            if (t_ofeliaLoadPath3d::pathSets[pos].polylines->at(i).inside(posX, posY)) {
                                
                                inside = true;
                                break;
                            }
                        }
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsPointInsidePath3d::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsPointInsidePath3d::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsPointInsidePath3d::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsPointInsidePath3d::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsPointInsidePath3d::objName);
    }
}

void ofeliaIsPointInsidePath3d_set(t_ofeliaIsPointInsidePath3d *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaIsPointInsidePath3d_print(t_ofeliaIsPointInsidePath3d *x)
{
    post("\n[%s]", t_ofeliaIsPointInsidePath3d::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaIsPointInsidePath3d_setup()
{
    ofeliaIsPointInsidePath3d_class = class_new(gensym("ofIsPointInsidePath3d"),
                                            reinterpret_cast<t_newmethod>(ofeliaIsPointInsidePath3d_new),
                                            0, sizeof(t_ofeliaIsPointInsidePath3d),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addlist(ofeliaIsPointInsidePath3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath3d_list));
    class_addmethod(ofeliaIsPointInsidePath3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath3d_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePath3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath3d_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsidePath3d_class, reinterpret_cast<t_method>(ofeliaIsPointInsidePath3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dCommand object methods
 */
void *ofeliaGetPath2dCommand_new(t_symbol *s)
{
    t_ofeliaGetPath2dCommand *x = reinterpret_cast<t_ofeliaGetPath2dCommand*>(pd_new(ofeliaGetPath2dCommand_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath2dCommand_bang(t_ofeliaGetPath2dCommand *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadPath2d::pathData[pos].elems.size())-1);
                vector<t_ofeliaAtomElem> elems;
                t_string name;
                getLoadPathCmdTypeString(t_ofeliaLoadPath2d::pathData[pos].elems[index].type, name);
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                
                for (int i=0; i<t_ofeliaLoadPath2d::pathData[pos].elems[index].numArgs; ++i)
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPath2d::pathData[pos].elems[index].args[i]});
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dCommand::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dCommand::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dCommand::objName);
    }
}

void ofeliaGetPath2dCommand_float(t_ofeliaGetPath2dCommand *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPath2dCommand_set(t_ofeliaGetPath2dCommand *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPath2dCommand_print(t_ofeliaGetPath2dCommand *x)
{
    post("\n[%s]", t_ofeliaGetPath2dCommand::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPath2dCommand_setup()
{
    ofeliaGetPath2dCommand_class = class_new(gensym("ofGetPath2dCommand"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetPath2dCommand_new),
                                             0, sizeof(t_ofeliaGetPath2dCommand),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath2dCommand_new),
                     gensym("ofGetPath2dCmd"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommand_bang));
    class_addfloat(ofeliaGetPath2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommand_float));
    class_addmethod(ofeliaGetPath2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommand_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommand_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommand_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dCommand object methods
 */
void *ofeliaGetPath3dCommand_new(t_symbol *s)
{
    t_ofeliaGetPath3dCommand *x = reinterpret_cast<t_ofeliaGetPath3dCommand*>(pd_new(ofeliaGetPath3dCommand_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath3dCommand_bang(t_ofeliaGetPath3dCommand *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadPath3d::pathData[pos].elems.size())-1);
                vector<t_ofeliaAtomElem> elems;
                t_string name;
                getLoadPathCmdTypeString(t_ofeliaLoadPath3d::pathData[pos].elems[index].type, name);
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                
                for (int i=0; i<t_ofeliaLoadPath3d::pathData[pos].elems[index].numArgs; ++i)
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPath3d::pathData[pos].elems[index].args[i]});
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dCommand::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dCommand::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dCommand::objName);
    }
}

void ofeliaGetPath3dCommand_float(t_ofeliaGetPath3dCommand *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetPath3dCommand_set(t_ofeliaGetPath3dCommand *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetPath3dCommand_print(t_ofeliaGetPath3dCommand *x)
{
    post("\n[%s]", t_ofeliaGetPath3dCommand::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetPath3dCommand_setup()
{
    ofeliaGetPath3dCommand_class = class_new(gensym("ofGetPath3dCommand"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetPath3dCommand_new),
                                             0, sizeof(t_ofeliaGetPath3dCommand),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath3dCommand_new),
                     gensym("ofGetPath3dCmd"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommand_bang));
    class_addfloat(ofeliaGetPath3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommand_float));
    class_addmethod(ofeliaGetPath3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommand_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommand_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCommand_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommand_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dCommands object methods
 */
void *ofeliaGetPath2dCommands_new(t_symbol *s)
{
    t_ofeliaGetPath2dCommands *x = reinterpret_cast<t_ofeliaGetPath2dCommands*>(pd_new(ofeliaGetPath2dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath2dCommands_bang(t_ofeliaGetPath2dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                LoadPathCmdType prevType = PATH_LOAD_CMDTYPE_UNKNOWN;

                for (size_t i=0; i<t_ofeliaLoadPath2d::pathData[pos].elems.size(); ++i) {
                    
                    t_string name;
                    const LoadPathCmdType type = t_ofeliaLoadPath2d::pathData[pos].elems[i].type;
                    
                    if (type != prevType) {
                        
                        getLoadPathCmdTypeString(type, name);
                        elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                        prevType = type;
                    }
                    for (int j=0; j<t_ofeliaLoadPath2d::pathData[pos].elems[i].numArgs; ++j)
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPath2d::pathData[pos].elems[i].args[j]});
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dCommands::objName);
    }
}

void ofeliaGetPath2dCommands_set(t_ofeliaGetPath2dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dCommands_print(t_ofeliaGetPath2dCommands *x)
{
    post("\n[%s]", t_ofeliaGetPath2dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dCommands_setup()
{
    ofeliaGetPath2dCommands_class = class_new(gensym("ofGetPath2dCommands"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPath2dCommands_new),
                                              0, sizeof(t_ofeliaGetPath2dCommands),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath2dCommands_new),
                     gensym("ofGetPath2dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommands_bang));
    class_addmethod(ofeliaGetPath2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath2dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dCommands object methods
 */
void *ofeliaGetPath3dCommands_new(t_symbol *s)
{
    t_ofeliaGetPath3dCommands *x = reinterpret_cast<t_ofeliaGetPath3dCommands*>(pd_new(ofeliaGetPath3dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath3dCommands_bang(t_ofeliaGetPath3dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                LoadPathCmdType prevType = PATH_LOAD_CMDTYPE_UNKNOWN;
                
                for (size_t i=0; i<t_ofeliaLoadPath3d::pathData[pos].elems.size(); ++i) {
                    
                    t_string name;
                    const LoadPathCmdType type = t_ofeliaLoadPath3d::pathData[pos].elems[i].type;
                    
                    if (type != prevType) {
                        
                        getLoadPathCmdTypeString(type, name);
                        elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, name});
                        prevType = type;
                    }
                    for (int j=0; j<t_ofeliaLoadPath3d::pathData[pos].elems[i].numArgs; ++j)
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, t_ofeliaLoadPath3d::pathData[pos].elems[i].args[j]});
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dCommands::objName);
    }
}

void ofeliaGetPath3dCommands_set(t_ofeliaGetPath3dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dCommands_print(t_ofeliaGetPath3dCommands *x)
{
    post("\n[%s]", t_ofeliaGetPath3dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dCommands_setup()
{
    ofeliaGetPath3dCommands_class = class_new(gensym("ofGetPath3dCommands"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPath3dCommands_new),
                                              0, sizeof(t_ofeliaGetPath3dCommands),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath3dCommands_new),
                     gensym("ofGetPath3dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommands_bang));
    class_addmethod(ofeliaGetPath3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCommands_class, reinterpret_cast<t_method>(ofeliaGetPath3dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dTessellation object methods
 */
void *ofeliaGetPath2dTessellation_new(t_symbol *s)
{
    t_ofeliaGetPath2dTessellation *x = reinterpret_cast<t_ofeliaGetPath2dTessellation*>(pd_new(ofeliaGetPath2dTessellation_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath2dTessellation_bang(t_ofeliaGetPath2dTessellation *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                const size_t numVertices = t_ofeliaLoadPath2d::pathSets[pos].mesh->getNumVertices();
                const size_t numIndices = t_ofeliaLoadPath2d::pathSets[pos].mesh->getNumIndices();
                
                if (numVertices) {
                    
                    elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                    
                    for (size_t i=0; i<numVertices; ++i) {
                        
                        const ofVec2f &vertex = t_ofeliaLoadPath2d::pathSets[pos].mesh->getVertex(i);
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x});
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y});
                    }
                }
                if (numIndices) {
                    
                    elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                    
                    for (size_t i=0; i<numIndices; ++i) {
                        
                        const ofIndexType index = t_ofeliaLoadPath2d::pathSets[pos].mesh->getIndex(i);
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(index)});
                    }
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dTessellation::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dTessellation::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dTessellation::objName);
    }
}

void ofeliaGetPath2dTessellation_set(t_ofeliaGetPath2dTessellation *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dTessellation_print(t_ofeliaGetPath2dTessellation *x)
{
    post("\n[%s]", t_ofeliaGetPath2dTessellation::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dTessellation_setup()
{
    ofeliaGetPath2dTessellation_class = class_new(gensym("ofGetPath2dTessellation"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPath2dTessellation_new),
                                                  0, sizeof(t_ofeliaGetPath2dTessellation),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath2dTessellation_new),
                     gensym("ofGetPath2dTess"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath2dTessellation_bang));
    class_addmethod(ofeliaGetPath2dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath2dTessellation_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath2dTessellation_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath2dTessellation_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dTessellation object methods
 */
void *ofeliaGetPath3dTessellation_new(t_symbol *s)
{
    t_ofeliaGetPath3dTessellation *x = reinterpret_cast<t_ofeliaGetPath3dTessellation*>(pd_new(ofeliaGetPath3dTessellation_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetPath3dTessellation_bang(t_ofeliaGetPath3dTessellation *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                vector<t_ofeliaAtomElem> elems;
                const size_t numVertices = t_ofeliaLoadPath3d::pathSets[pos].mesh->getNumVertices();
                const size_t numIndices = t_ofeliaLoadPath3d::pathSets[pos].mesh->getNumIndices();
                
                if (numVertices) {
                    
                    elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                    
                    for (size_t i=0; i<numVertices; ++i) {
                        
                        const ofVec3f &vertex = t_ofeliaLoadPath3d::pathSets[pos].mesh->getVertex(i);
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x});
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y});
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.z});
                    }
                }
                if (numIndices) {
                    
                    elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                    
                    for (size_t i=0; i<numIndices; ++i) {
                        
                        const ofIndexType index = t_ofeliaLoadPath3d::pathSets[pos].mesh->getIndex(i);
                        elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(index)});
                    }
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
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dTessellation::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dTessellation::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dTessellation::objName);
    }
}

void ofeliaGetPath3dTessellation_set(t_ofeliaGetPath3dTessellation *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dTessellation_print(t_ofeliaGetPath3dTessellation *x)
{
    post("\n[%s]", t_ofeliaGetPath3dTessellation::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dTessellation_setup()
{
    ofeliaGetPath3dTessellation_class = class_new(gensym("ofGetPath3dTessellation"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetPath3dTessellation_new),
                                                  0, sizeof(t_ofeliaGetPath3dTessellation),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPath3dTessellation_new),
                     gensym("ofGetPath3dTess"), A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath3dTessellation_bang));
    class_addmethod(ofeliaGetPath3dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath3dTessellation_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath3dTessellation_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dTessellation_class, reinterpret_cast<t_method>(ofeliaGetPath3dTessellation_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dBoundingBox object methods
 */
void *ofeliaGetPath2dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetPath2dBoundingBox *x = reinterpret_cast<t_ofeliaGetPath2dBoundingBox*>(pd_new(ofeliaGetPath2dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath2dBoundingBox_bang(t_ofeliaGetPath2dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                    updatePath2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPath2d::pathData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPath2d::pathData[pos].boxDimen.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dBoundingBox::objName);
    }
}

void ofeliaGetPath2dBoundingBox_set(t_ofeliaGetPath2dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dBoundingBox_print(t_ofeliaGetPath2dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetPath2dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dBoundingBox_setup()
{
    ofeliaGetPath2dBoundingBox_class = class_new(gensym("ofGetPath2dBoundingBox"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetPath2dBoundingBox_new),
                                                 0, sizeof(t_ofeliaGetPath2dBoundingBox),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath2dBoundingBox_bang));
    class_addmethod(ofeliaGetPath2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath2dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath2dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath2dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dBoundingBox object methods
 */
void *ofeliaGetPath3dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetPath3dBoundingBox *x = reinterpret_cast<t_ofeliaGetPath3dBoundingBox*>(pd_new(ofeliaGetPath3dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath3dBoundingBox_bang(t_ofeliaGetPath3dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                    updatePath3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].boxDimen.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].boxDimen.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dBoundingBox::objName);
    }
}

void ofeliaGetPath3dBoundingBox_set(t_ofeliaGetPath3dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dBoundingBox_print(t_ofeliaGetPath3dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetPath3dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dBoundingBox_setup()
{
    ofeliaGetPath3dBoundingBox_class = class_new(gensym("ofGetPath3dBoundingBox"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetPath3dBoundingBox_new),
                                                 0, sizeof(t_ofeliaGetPath3dBoundingBox),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath3dBoundingBox_bang));
    class_addmethod(ofeliaGetPath3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath3dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath3dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetPath3dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dCentroid object methods
 */
void *ofeliaGetPath2dCentroid_new(t_symbol *s)
{
    t_ofeliaGetPath2dCentroid *x = reinterpret_cast<t_ofeliaGetPath2dCentroid*>(pd_new(ofeliaGetPath2dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath2dCentroid_bang(t_ofeliaGetPath2dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                    updatePath2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPath2d::pathData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPath2d::pathData[pos].centroid.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dCentroid::objName);
    }
}

void ofeliaGetPath2dCentroid_set(t_ofeliaGetPath2dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dCentroid_print(t_ofeliaGetPath2dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetPath2dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dCentroid_setup()
{
    ofeliaGetPath2dCentroid_class = class_new(gensym("ofGetPath2dCentroid"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPath2dCentroid_new),
                                              0, sizeof(t_ofeliaGetPath2dCentroid),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath2dCentroid_bang));
    class_addmethod(ofeliaGetPath2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath2dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath2dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath2dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dCentroid object methods
 */
void *ofeliaGetPath3dCentroid_new(t_symbol *s)
{
    t_ofeliaGetPath3dCentroid *x = reinterpret_cast<t_ofeliaGetPath3dCentroid*>(pd_new(ofeliaGetPath3dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetPath3dCentroid_bang(t_ofeliaGetPath3dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                    updatePath3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].centroid.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadPath3d::pathData[pos].centroid.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dCentroid::objName);
    }
}

void ofeliaGetPath3dCentroid_set(t_ofeliaGetPath3dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dCentroid_print(t_ofeliaGetPath3dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetPath3dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dCentroid_setup()
{
    ofeliaGetPath3dCentroid_class = class_new(gensym("ofGetPath3dCentroid"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPath3dCentroid_new),
                                              0, sizeof(t_ofeliaGetPath3dCentroid),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath3dCentroid_bang));
    class_addmethod(ofeliaGetPath3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath3dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath3dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetPath3dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dArea object methods
 */
void *ofeliaGetPath2dArea_new(t_symbol *s)
{
    t_ofeliaGetPath2dArea *x = reinterpret_cast<t_ofeliaGetPath2dArea*>(pd_new(ofeliaGetPath2dArea_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPath2dArea_bang(t_ofeliaGetPath2dArea *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                    updatePath2dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPath2d::pathData[pos].area);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dArea::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dArea::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dArea::objName);
    }
}

void ofeliaGetPath2dArea_set(t_ofeliaGetPath2dArea *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dArea_print(t_ofeliaGetPath2dArea *x)
{
    post("\n[%s]", t_ofeliaGetPath2dArea::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dArea_setup()
{
    ofeliaGetPath2dArea_class = class_new(gensym("ofGetPath2dArea"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetPath2dArea_new),
                                          0, sizeof(t_ofeliaGetPath2dArea),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dArea_class, reinterpret_cast<t_method>(ofeliaGetPath2dArea_bang));
    class_addmethod(ofeliaGetPath2dArea_class, reinterpret_cast<t_method>(ofeliaGetPath2dArea_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dArea_class, reinterpret_cast<t_method>(ofeliaGetPath2dArea_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dArea_class, reinterpret_cast<t_method>(ofeliaGetPath2dArea_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dArea object methods
 */
void *ofeliaGetPath3dArea_new(t_symbol *s)
{
    t_ofeliaGetPath3dArea *x = reinterpret_cast<t_ofeliaGetPath3dArea*>(pd_new(ofeliaGetPath3dArea_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPath3dArea_bang(t_ofeliaGetPath3dArea *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                    updatePath3dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPath3d::pathData[pos].area);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dArea::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dArea::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dArea::objName);
    }
}

void ofeliaGetPath3dArea_set(t_ofeliaGetPath3dArea *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dArea_print(t_ofeliaGetPath3dArea *x)
{
    post("\n[%s]", t_ofeliaGetPath3dArea::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dArea_setup()
{
    ofeliaGetPath3dArea_class = class_new(gensym("ofGetPath3dArea"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetPath3dArea_new),
                                          0, sizeof(t_ofeliaGetPath3dArea),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dArea_class, reinterpret_cast<t_method>(ofeliaGetPath3dArea_bang));
    class_addmethod(ofeliaGetPath3dArea_class, reinterpret_cast<t_method>(ofeliaGetPath3dArea_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dArea_class, reinterpret_cast<t_method>(ofeliaGetPath3dArea_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dArea_class, reinterpret_cast<t_method>(ofeliaGetPath3dArea_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath2dPerimeter object methods
 */
void *ofeliaGetPath2dPerimeter_new(t_symbol *s)
{
    t_ofeliaGetPath2dPerimeter *x = reinterpret_cast<t_ofeliaGetPath2dPerimeter*>(pd_new(ofeliaGetPath2dPerimeter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPath2dPerimeter_bang(t_ofeliaGetPath2dPerimeter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath2dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath2d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath2d::pathData[pos].shouldUpdateProperties)
                    updatePath2dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPath2d::pathData[pos].perimeter);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath2dPerimeter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath2dPerimeter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath2dPerimeter::objName);
    }
}

void ofeliaGetPath2dPerimeter_set(t_ofeliaGetPath2dPerimeter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath2dPerimeter_print(t_ofeliaGetPath2dPerimeter *x)
{
    post("\n[%s]", t_ofeliaGetPath2dPerimeter::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath2dPerimeter_setup()
{
    ofeliaGetPath2dPerimeter_class = class_new(gensym("ofGetPath2dPerimeter"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetPath2dPerimeter_new),
                                               0, sizeof(t_ofeliaGetPath2dPerimeter),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath2dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath2dPerimeter_bang));
    class_addmethod(ofeliaGetPath2dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath2dPerimeter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath2dPerimeter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath2dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath2dPerimeter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetPath3dPerimeter object methods
 */
void *ofeliaGetPath3dPerimeter_new(t_symbol *s)
{
    t_ofeliaGetPath3dPerimeter *x = reinterpret_cast<t_ofeliaGetPath3dPerimeter*>(pd_new(ofeliaGetPath3dPerimeter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPath3dPerimeter_bang(t_ofeliaGetPath3dPerimeter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByPath3dName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadPath3d::pathData[pos].elems.empty()) {
                
                if (t_ofeliaLoadPath3d::pathData[pos].shouldUpdateProperties)
                    updatePath3dProperties(pos);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadPath3d::pathData[pos].perimeter);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetPath3dPerimeter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetPath3dPerimeter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetPath3dPerimeter::objName);
    }
}

void ofeliaGetPath3dPerimeter_set(t_ofeliaGetPath3dPerimeter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetPath3dPerimeter_print(t_ofeliaGetPath3dPerimeter *x)
{
    post("\n[%s]", t_ofeliaGetPath3dPerimeter::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetPath3dPerimeter_setup()
{
    ofeliaGetPath3dPerimeter_class = class_new(gensym("ofGetPath3dPerimeter"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetPath3dPerimeter_new),
                                               0, sizeof(t_ofeliaGetPath3dPerimeter),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetPath3dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath3dPerimeter_bang));
    class_addmethod(ofeliaGetPath3dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath3dPerimeter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath3dPerimeter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetPath3dPerimeter_class, reinterpret_cast<t_method>(ofeliaGetPath3dPerimeter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaPath_setup()
{
    ofeliaLoadPath2d_setup();
    ofeliaLoadPath3d_setup();
    ofeliaDrawPath2d_setup();
    ofeliaDrawPath3d_setup();
    ofeliaDoesPath2dNameExist_setup();
    ofeliaDoesPath3dNameExist_setup();
    ofeliaGetPath2dPoint_setup();
    ofeliaGetPath3dPoint_setup();
    ofeliaGetPath2dPoints_setup();
    ofeliaGetPath3dPoints_setup();
    ofeliaIsPointInsidePath2d_setup();
    ofeliaIsPointInsidePath3d_setup();
    ofeliaGetPath2dCommand_setup();
    ofeliaGetPath3dCommand_setup();
    ofeliaGetPath2dCommands_setup();
    ofeliaGetPath3dCommands_setup();
    ofeliaGetPath2dTessellation_setup();
    ofeliaGetPath3dTessellation_setup();
    ofeliaGetPath2dBoundingBox_setup();
    ofeliaGetPath3dBoundingBox_setup();
    ofeliaGetPath2dCentroid_setup();
    ofeliaGetPath3dCentroid_setup();
    ofeliaGetPath2dArea_setup();
    ofeliaGetPath3dArea_setup();
    ofeliaGetPath2dPerimeter_setup();
    ofeliaGetPath3dPerimeter_setup();
}

