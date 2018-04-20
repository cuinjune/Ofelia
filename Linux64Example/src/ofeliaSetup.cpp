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

#include "ofeliaSetup.h"
#include "ofeliaWindow.h"
#include "ofeliaListeners.h"
#include "ofeliaGetters.h"
#include "ofeliaSetters.h"
#include "ofeliaPatch.h"
#include "ofeliaDevices.h"
#include "ofeliaAudio.h"
#include "ofeliaUtils.h"
#include "ofeliaAnimate.h"
#include "ofeliaExpr.h"
#include "ofeliaDirectory.h"
#include "ofeliaFile.h"
#include "ofeliaURL.h"
#include "ofeliaMath.h"
#include "ofeliaFloat.h"
#include "ofeliaVec2f.h"
#include "ofeliaVec3f.h"
#include "ofeliaVec4f.h"
#include "ofeliaColor.h"
#include "ofeliaRect.h"
#include "ofeliaSymbol.h"
#include "ofeliaHead.h"
#include "ofeliaSeparators.h"
#include "ofeliaStyle.h"
#include "ofeliaMatrix.h"
#include "ofeliaFont.h"
#include "ofeliaFbo.h"
#include "ofeliaImage.h"
#include "ofeliaShader.h"
#include "ofeliaMesh.h"
#include "ofeliaPath.h"
#include "ofeliaPolyline.h"
#include "ofeliaShapes.h"
#include "ofeliaMaterial.h"
#include "ofeliaLights.h"
#include "ofeliaEasyCam.h"
#include "ofeliaCamera.h"
#include "ofeliaViewport.h"

void *ofelia_new() {
    
    t_ofelia *x = reinterpret_cast<t_ofelia*>(pd_new(ofelia_class));
    return (x);
}

void ofelia_setup()
{
    ofelia_class = class_new(gensym("ofelia"), reinterpret_cast<t_newmethod>(ofelia_new), 0,
                             sizeof(t_ofelia), CLASS_NOINLET, A_NULL, 0);
    ofeliaWindow_setup();
    ofeliaListeners_setup();
    ofeliaGetters_setup();
    ofeliaSetters_setup();
    ofeliaPatch_setup();
    ofeliaDevices_setup();
    ofeliaAudio_setup();
    ofeliaUtils_setup();
    ofeliaAnimate_setup();
    ofeliaExpr_setup();
    ofeliaDirectory_setup();
    ofeliaFile_setup();
    ofeliaURL_setup();
    ofeliaMath_setup();
    ofeliaFloat_setup();
    ofeliaVec2f_setup();
    ofeliaVec3f_setup();
    ofeliaVec4f_setup();
    ofeliaColor_setup();
    ofeliaRect_setup();
    ofeliaSymbol_setup();
    ofeliaHead_setup();
    ofeliaSeparators_setup();
    ofeliaStyle_setup();
    ofeliaMatrix_setup();
    ofeliaFont_setup();
    ofeliaFbo_setup();
    ofeliaImage_setup();
    ofeliaShader_setup();
    ofeliaMesh_setup();
    ofeliaPath_setup();
    ofeliaPolyline_setup();
    ofeliaShapes_setup();
    ofeliaMaterial_setup();
    ofeliaLights_setup();
    ofeliaEasyCam_setup();
    ofeliaCamera_setup();
    ofeliaViewport_setup();
    stringstream ss;
    ss << "-------------------------------------------------------------------" << '\n';
    ss << "ofelia" << ' ';
    ss << 'v';
    ss << OFELIA_MAJOR_VERSION << '.';
    ss << OFELIA_MINOR_VERSION << '.';
    ss << OFELIA_BUGFIX_VERSION << ':';
    ss << " compiled on " << __DATE__ << '\n';
    ss << "(c) 2018 Zack Lee <cuinjune@gmail.com>" << '\n';
    ss << "-------------------------------------------------------------------";
    post(ss.str().c_str());
}

