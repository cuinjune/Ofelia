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

#include "ofeliaStyle.h"
#include "ofeliaWindow.h"
#include "ofeliaGetters.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaPushStyle::objName = "ofPushStyle";
const char *t_ofeliaPopStyle::objName = "ofPopStyle";
const char *t_ofeliaSetColor::objName = "ofSetColor";
const char *t_ofeliaSetBgColor::objName = "ofSetBgColor";
const char *t_ofeliaSetRectMode::objName = "ofSetRectMode";
const char *t_ofeliaSetTextMode::objName = "ofSetTextMode";
vector<t_ofeliaTextModeElem> t_ofeliaSetTextMode::textModeVec;
const char *t_ofeliaSetFillMode::objName = "ofSetFillMode";
const char *t_ofeliaSetPolyMode::objName = "ofSetPolyMode";
const char *t_ofeliaSetBlendMode::objName = "ofSetBlendMode";
const char *t_ofeliaSetLineWidth::objName = "ofSetLineWidth";
const char *t_ofeliaSetLineSmoothing::objName = "ofSetLineSmoothing";
const char *t_ofeliaSetCircleRes::objName = "ofSetCircleRes";
const char *t_ofeliaSetCurveRes::objName = "ofSetCurveRes";

bool t_ofeliaSetColor::getColorByName(const char *colorName, ofColor &color)
{
    if (!strncmp(colorName, "0x", 2)) {
        
        if (strlen(colorName) == 8) {
           
            color.setHex(ofHexToInt(colorName));
            return 1;
        }
        else {
            
            error("%s: invalid hex color '%s'", t_ofeliaSetColor::objName, colorName);
            return 0;
        }
    }
    if (!strcmp(colorName, "white")) {
        
        color.set(ofColor::white);
        return 1;
    }
    if (!strcmp(colorName, "gray")) {
        
        color.set(ofColor::gray);
        return 1;
    }
    if (!strcmp(colorName, "grey")) {
        
        color.set(ofColor::grey);
        return 1;
    }
    if (!strcmp(colorName, "black")) {
        
        color.set(ofColor::black);
        return 1;
    }
    if (!strcmp(colorName, "red")) {
        
        color.set(ofColor::red);
        return 1;
    }
    if (!strcmp(colorName, "green")) {
        
        color.set(ofColor::green);
        return 1;
    }
    if (!strcmp(colorName, "blue")) {
        
        color.set(ofColor::blue);
        return 1;
    }
    if (!strcmp(colorName, "cyan")) {
        
        color.set(ofColor::cyan);
        return 1;
    }
    if (!strcmp(colorName, "magenta")) {
        
        color.set(ofColor::magenta);
        return 1;
    }
    if (!strcmp(colorName, "yellow")) {
        
        color.set(ofColor::yellow);
        return 1;
    }
    if (!strcmp(colorName, "aqua")) {
        
        color.set(ofColor::aqua);
        return 1;
    }
    if (!strcmp(colorName, "azure")) {
        
        color.set(ofColor::azure);
        return 1;
    }
    if (!strcmp(colorName, "beige")) {
        
        color.set(ofColor::beige);
        return 1;
    }
    if (!strcmp(colorName, "bisque")) {
        
        color.set(ofColor::bisque);
        return 1;
    }
    if (!strcmp(colorName, "brown")) {

        color.set(ofColor::brown);
        return 1;
    }
    if (!strcmp(colorName, "chartreuse")) {
        
        color.set(ofColor::chartreuse);
        return 1;
    }
    if (!strcmp(colorName, "chocolate")) {
        
        color.set(ofColor::chocolate);
        return 1;
    }
    if (!strcmp(colorName, "coral")) {
        
        color.set(ofColor::coral);
        return 1;
    }
    if (!strcmp(colorName, "cornsilk")) {
        
        color.set(ofColor::cornsilk);
        return 1;
    }
    if (!strcmp(colorName, "crimson")) {
        
        color.set(ofColor::crimson);
        return 1;
    }
    if (!strcmp(colorName, "fuchsia")) {
        
        color.set(ofColor::fuchsia);
        return 1;
    }
    if (!strcmp(colorName, "gainsboro")) {
        
        color.set(ofColor::gainsboro);
        return 1;
    }
    if (!strcmp(colorName, "gold")) {
        
        color.set(ofColor::gold);
        return 1;
    }
    if (!strcmp(colorName, "indigo")) {
        
        color.set(ofColor::indigo);
        return 1;
    }
    if (!strcmp(colorName, "ivory")) {
        
        color.set(ofColor::ivory);
        return 1;
    }
    if (!strcmp(colorName, "khaki")) {
        
        color.set(ofColor::khaki);
        return 1;
    }
    if (!strcmp(colorName, "lavender")) {
        
        color.set(ofColor::lavender);
        return 1;
    }
    if (!strcmp(colorName, "lime")) {
        
        color.set(ofColor::lime);
        return 1;
    }
    if (!strcmp(colorName, "linen")) {
        
        color.set(ofColor::linen);
        return 1;
    }
    if (!strcmp(colorName, "maroon")) {
        
        color.set(ofColor::maroon);
        return 1;
    }
    if (!strcmp(colorName, "moccasin")) {
        
        color.set(ofColor::moccasin);
        return 1;
    }
    if (!strcmp(colorName, "navy")) {
        
        color.set(ofColor::navy);
        return 1;
    }
    if (!strcmp(colorName, "olive")) {
        
        color.set(ofColor::olive);
        return 1;
    }
    if (!strcmp(colorName, "orange")) {
        
        color.set(ofColor::orange);
        return 1;
    }
    if (!strcmp(colorName, "orchid")) {
        
        color.set(ofColor::orchid);
        return 1;
    }
    if (!strcmp(colorName, "peru")) {
        
        color.set(ofColor::peru);
        return 1;
    }
    if (!strcmp(colorName, "pink")) {
        
        color.set(ofColor::pink);
        return 1;
    }
    if (!strcmp(colorName, "plum")) {
        
        color.set(ofColor::plum);
        return 1;
    }
    if (!strcmp(colorName, "purple")) {
        
        color.set(ofColor::purple);
        return 1;
    }
    if (!strcmp(colorName, "salmon")) {
        
        color.set(ofColor::salmon);
        return 1;
    }
    if (!strcmp(colorName, "sienna")) {
        
        color.set(ofColor::sienna);
        return 1;
    }
    if (!strcmp(colorName, "silver")) {
        
        color.set(ofColor::silver);
        return 1;
    }
    if (!strcmp(colorName, "snow")) {
        
        color.set(ofColor::snow);
        return 1;
    }
    if (!strcmp(colorName, "tan")) {
        
        color.set(ofColor::tan);
        return 1;
    }
    if (!strcmp(colorName, "teal")) {
        
        color.set(ofColor::teal);
        return 1;
    }
    if (!strcmp(colorName, "thistle")) {
        
        color.set(ofColor::thistle);
        return 1;
    }
    if (!strcmp(colorName, "tomato")) {
        
        color.set(ofColor::tomato);
        return 1;
    }
    if (!strcmp(colorName, "turquoise")) {
        
        color.set(ofColor::turquoise);
        return 1;
    }
    if (!strcmp(colorName, "violet")) {
        
        color.set(ofColor::violet);
        return 1;
    }
    if (!strcmp(colorName, "wheat")) {
        
        color.set(ofColor::wheat);
        return 1;
    }
    if (!strcmp(colorName, "aliceBlue")) {
        
        color.set(ofColor::aliceBlue);
        return 1;
    }
    if (!strcmp(colorName, "antiqueWhite")) {
        
        color.set(ofColor::antiqueWhite);
        return 1;
    }
    if (!strcmp(colorName, "aquamarine")) {
        
        color.set(ofColor::aquamarine);
        return 1;
    }
    if (!strcmp(colorName, "blanchedAlmond")) {
        
        color.set(ofColor::blanchedAlmond);
        return 1;
    }
    if (!strcmp(colorName, "blueViolet")) {
        
        color.set(ofColor::blueViolet);
        return 1;
    }
    if (!strcmp(colorName, "burlyWood")) {
        
        color.set(ofColor::burlyWood);
        return 1;
    }
    if (!strcmp(colorName, "cadetBlue")) {
        
        color.set(ofColor::cadetBlue);
        return 1;
    }
    if (!strcmp(colorName, "cornflowerBlue")) {
        
        color.set(ofColor::cornflowerBlue);
        return 1;
    }
    if (!strcmp(colorName, "darkBlue")) {
        
        color.set(ofColor::darkBlue);
        return 1;
    }
    if (!strcmp(colorName, "darkCyan")) {
        
        color.set(ofColor::darkCyan);
        return 1;
    }
    if (!strcmp(colorName, "darkGoldenRod")) {
        
        color.set(ofColor::darkGoldenRod);
        return 1;
    }
    if (!strcmp(colorName, "darkGray")) {
        
        color.set(ofColor::darkGray);
        return 1;
    }
    if (!strcmp(colorName, "darkGrey")) {
        
        color.set(ofColor::darkGrey);
        return 1;
    }
    if (!strcmp(colorName, "darkGreen")) {
        
        color.set(ofColor::darkGreen);
        return 1;
    }
    if (!strcmp(colorName, "darkKhaki")) {
        
        color.set(ofColor::darkKhaki);
        return 1;
    }
    if (!strcmp(colorName, "darkMagenta")) {
        
        color.set(ofColor::darkMagenta);
        return 1;
    }
    if (!strcmp(colorName, "darkOliveGreen")) {
        
        color.set(ofColor::darkOliveGreen);
        return 1;
    }
    if (!strcmp(colorName, "darkorange")) {

        color.set(ofColor::darkorange);
        return 1;
    }
    if (!strcmp(colorName, "darkOrchid")) {
        
        color.set(ofColor::darkOrchid);
        return 1;
    }
    if (!strcmp(colorName, "darkRed")) {
        
        color.set(ofColor::darkRed);
        return 1;
    }
    if (!strcmp(colorName, "darkSalmon")) {
        
        color.set(ofColor::darkSalmon);
        return 1;
    }
    if (!strcmp(colorName, "darkSeaGreen")) {
        
        color.set(ofColor::darkSeaGreen);
        return 1;
    }
    if (!strcmp(colorName, "darkSlateBlue")) {
        
        color.set(ofColor::darkSlateBlue);
        return 1;
    }
    if (!strcmp(colorName, "darkSlateGray")) {
        
        color.set(ofColor::darkSlateGray);
        return 1;
    }
    if (!strcmp(colorName, "darkSlateGrey")) {
        
        color.set(ofColor::darkSlateGrey);
        return 1;
    }
    if (!strcmp(colorName, "darkTurquoise")) {
        
        color.set(ofColor::darkTurquoise);
        return 1;
    }
    if (!strcmp(colorName, "darkViolet")) {
        
        color.set(ofColor::darkViolet);
        return 1;
    }
    if (!strcmp(colorName, "deepPink")) {
        
        color.set(ofColor::deepPink);
        return 1;
    }
    if (!strcmp(colorName, "deepSkyBlue")) {
        
        color.set(ofColor::deepSkyBlue);
        return 1;
    }
    if (!strcmp(colorName, "dimGray")) {
        
        color.set(ofColor::dimGray);
        return 1;
    }
    if (!strcmp(colorName, "dimGrey")) {
        
        color.set(ofColor::dimGrey);
        return 1;
    }
    if (!strcmp(colorName, "dodgerBlue")) {
        
        color.set(ofColor::dodgerBlue);
        return 1;
    }
    if (!strcmp(colorName, "fireBrick")) {
        
        color.set(ofColor::fireBrick);
        return 1;
    }
    if (!strcmp(colorName, "floralWhite")) {
        
        color.set(ofColor::floralWhite);
        return 1;
    }
    if (!strcmp(colorName, "forestGreen")) {
        
        color.set(ofColor::forestGreen);
        return 1;
    }
    if (!strcmp(colorName, "ghostWhite")) {
        
        color.set(ofColor::ghostWhite);
        return 1;
    }
    if (!strcmp(colorName, "goldenRod")) {
        
        color.set(ofColor::goldenRod);
        return 1;
    }
    if (!strcmp(colorName, "greenYellow")) {
        
        color.set(ofColor::greenYellow);
        return 1;
    }
    if (!strcmp(colorName, "honeyDew")) {
        
        color.set(ofColor::honeyDew);
        return 1;
    }
    if (!strcmp(colorName, "hotPink")) {
        
        color.set(ofColor::hotPink);
        return 1;
    }
    if (!strcmp(colorName, "indianRed")) {
        
        color.set(ofColor::indianRed);
        return 1;
    }
    if (!strcmp(colorName, "lavenderBlush")) {
        
        color.set(ofColor::lavenderBlush);
        return 1;
    }
    if (!strcmp(colorName, "lawnGreen")) {
        
        color.set(ofColor::lawnGreen);
        return 1;
    }
    if (!strcmp(colorName, "lemonChiffon")) {
        
        color.set(ofColor::lemonChiffon);
        return 1;
    }
    if (!strcmp(colorName, "lightBlue")) {
        
        color.set(ofColor::lightBlue);
        return 1;
    }
    if (!strcmp(colorName, "lightCoral")) {
        
        color.set(ofColor::lightCoral);
        return 1;
    }
    if (!strcmp(colorName, "lightCyan")) {
        
        color.set(ofColor::lightCyan);
        return 1;
    }
    if (!strcmp(colorName, "lightGoldenRodYellow")) {
        
        color.set(ofColor::lightGoldenRodYellow);
        return 1;
    }
    if (!strcmp(colorName, "lightGray")) {
        
        color.set(ofColor::lightGray);
        return 1;
    }
    if (!strcmp(colorName, "lightGrey")) {
        
        color.set(ofColor::lightGrey);
        return 1;
    }
    if (!strcmp(colorName, "lightGreen")) {
        
        color.set(ofColor::lightGreen);
        return 1;
    }
    if (!strcmp(colorName, "lightPink")) {
        
        color.set(ofColor::lightPink);
        return 1;
    }
    if (!strcmp(colorName, "lightSalmon")) {
        
        color.set(ofColor::lightSalmon);
        return 1;
    }
    if (!strcmp(colorName, "lightSeaGreen")) {
        
        color.set(ofColor::lightSeaGreen);
        return 1;
    }
    if (!strcmp(colorName, "lightSkyBlue")) {
        
        color.set(ofColor::lightSkyBlue);
        return 1;
    }
    if (!strcmp(colorName, "lightSlateGray")) {
        
        color.set(ofColor::lightSlateGray);
        return 1;
    }
    if (!strcmp(colorName, "lightSlateGrey")) {
        
        color.set(ofColor::lightSlateGrey);
        return 1;
    }
    if (!strcmp(colorName, "lightSteelBlue")) {
        
        color.set(ofColor::lightSteelBlue);
        return 1;
    }
    if (!strcmp(colorName, "lightYellow")) {
        
        color.set(ofColor::lightYellow);
        return 1;
    }
    if (!strcmp(colorName, "limeGreen")) {
        
        color.set(ofColor::limeGreen);
        return 1;
    }
    if (!strcmp(colorName, "mediumAquaMarine")) {
        
        color.set(ofColor::mediumAquaMarine);
        return 1;
    }
    if (!strcmp(colorName, "mediumBlue")) {
        
        color.set(ofColor::mediumBlue);
        return 1;
    }
    if (!strcmp(colorName, "mediumOrchid")) {
        
        color.set(ofColor::mediumOrchid);
        return 1;
    }
    if (!strcmp(colorName, "mediumPurple")) {
        
        color.set(ofColor::mediumPurple);
        return 1;
    }
    if (!strcmp(colorName, "mediumSeaGreen")) {
        
        color.set(ofColor::mediumSeaGreen);
        return 1;
    }
    if (!strcmp(colorName, "mediumSlateBlue")) {
        
        color.set(ofColor::mediumSlateBlue);
        return 1;
    }
    if (!strcmp(colorName, "mediumSpringGreen")) {
        
        color.set(ofColor::mediumSpringGreen);
        return 1;
    }
    if (!strcmp(colorName, "mediumTurquoise")) {
        
        color.set(ofColor::mediumTurquoise);
        return 1;
    }
    if (!strcmp(colorName, "mediumVioletRed")) {
        
        color.set(ofColor::mediumVioletRed);
        return 1;
    }
    if (!strcmp(colorName, "midnightBlue")) {
        
        color.set(ofColor::midnightBlue);
        return 1;
    }
    if (!strcmp(colorName, "mintCream")) {
        
        color.set(ofColor::mintCream);
        return 1;
    }
    if (!strcmp(colorName, "mistyRose")) {
        
        color.set(ofColor::mistyRose);
        return 1;
    }
    if (!strcmp(colorName, "navajoWhite")) {
        
        color.set(ofColor::navajoWhite);
        return 1;
    }
    if (!strcmp(colorName, "oldLace")) {
        
        color.set(ofColor::oldLace);
        return 1;
    }
    if (!strcmp(colorName, "oliveDrab")) {
        
        color.set(ofColor::oliveDrab);
        return 1;
    }
    if (!strcmp(colorName, "orangeRed")) {
        
        color.set(ofColor::orangeRed);
        return 1;
    }
    if (!strcmp(colorName, "paleGoldenRod")) {
        
        color.set(ofColor::paleGoldenRod);
        return 1;
    }
    if (!strcmp(colorName, "paleGreen")) {
        
        color.set(ofColor::paleGreen);
        return 1;
    }
    if (!strcmp(colorName, "paleTurquoise")) {
        
        color.set(ofColor::paleTurquoise);
        return 1;
    }
    if (!strcmp(colorName, "paleVioletRed")) {
        
        color.set(ofColor::paleVioletRed);
        return 1;
    }
    if (!strcmp(colorName, "papayaWhip")) {
        
        color.set(ofColor::papayaWhip);
        return 1;
    }
    if (!strcmp(colorName, "peachPuff")) {
        
        color.set(ofColor::peachPuff);
        return 1;
    }
    if (!strcmp(colorName, "powderBlue")) {
        
        color.set(ofColor::powderBlue);
        return 1;
    }
    if (!strcmp(colorName, "rosyBrown")) {
        
        color.set(ofColor::rosyBrown);
        return 1;
    }
    if (!strcmp(colorName, "royalBlue")) {
        
        color.set(ofColor::royalBlue);
        return 1;
    }
    if (!strcmp(colorName, "saddleBrown")) {
        
        color.set(ofColor::saddleBrown);
        return 1;
    }
    if (!strcmp(colorName, "sandyBrown")) {
        
        color.set(ofColor::sandyBrown);
        return 1;
    }
    if (!strcmp(colorName, "seaGreen")) {
        
        color.set(ofColor::seaGreen);
        return 1;
    }
    if (!strcmp(colorName, "seaShell")) {
        
        color.set(ofColor::seaShell);
        return 1;
    }
    if (!strcmp(colorName, "skyBlue")) {
        
        color.set(ofColor::skyBlue);
        return 1;
    }
    if (!strcmp(colorName, "slateBlue")) {
        
        color.set(ofColor::slateBlue);
        return 1;
    }
    if (!strcmp(colorName, "slateGray")) {
        
        color.set(ofColor::slateGray);
        return 1;
    }
    if (!strcmp(colorName, "slateGrey")) {
        
        color.set(ofColor::slateGrey);
        return 1;
    }
    if (!strcmp(colorName, "springGreen")) {
        
        color.set(ofColor::springGreen);
        return 1;
    }
    if (!strcmp(colorName, "steelBlue")) {
        
        color.set(ofColor::steelBlue);
        return 1;
    }
    if (!strcmp(colorName, "blueSteel")) {
        
        color.set(ofColor::blueSteel);
        return 1;
    }
    if (!strcmp(colorName, "whiteSmoke")) {
        
        color.set(ofColor::whiteSmoke);
        return 1;
    }
    if (!strcmp(colorName, "yellowGreen")) {
        
        color.set(ofColor::yellowGreen);
        return 1;
    }
    error("%s: invalid color name", t_ofeliaSetColor::objName);
    return 0;
}

/* ________________________________________________________________________________
 * ofPushStyle object methods
 */
void *ofeliaPushStyle_new()
{
    t_ofeliaPushStyle *x = reinterpret_cast<t_ofeliaPushStyle*>(pd_new(ofeliaPushStyle_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPushStyle_bang(t_ofeliaPushStyle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        t_ofeliaSetTextMode::textModeVec.push_back(t_ofeliaSetTextMode::textModeVec.back());
        ofGetCurrentRenderer()->pushStyle();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPushStyle::objName);
    }
}

void ofeliaPushStyle_setup()
{
    ofeliaPushStyle_class = class_new(gensym("ofPushStyle"),
                                      reinterpret_cast<t_newmethod>(ofeliaPushStyle_new),
                                      0, sizeof(t_ofeliaPushStyle),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaPushStyle_class, reinterpret_cast<t_method>(ofeliaPushStyle_bang));
}

/* ________________________________________________________________________________
 * ofPopStyle object methods
 */
void *ofeliaPopStyle_new()
{
    t_ofeliaPopStyle *x = reinterpret_cast<t_ofeliaPopStyle*>(pd_new(ofeliaPopStyle_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPopStyle_bang(t_ofeliaPopStyle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->popStyle();
        t_ofeliaSetTextMode::textModeVec.pop_back();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPopStyle::objName);
    }
}

void ofeliaPopStyle_setup()
{
    ofeliaPopStyle_class = class_new(gensym("ofPopStyle"),
                                     reinterpret_cast<t_newmethod>(ofeliaPopStyle_new),
                                     0, sizeof(t_ofeliaPopStyle),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaPopStyle_class, reinterpret_cast<t_method>(ofeliaPopStyle_bang));
}

/* ________________________________________________________________________________
 * ofSetColor object methods
 */
void *ofeliaSetColor_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetColor *x = reinterpret_cast<t_ofeliaSetColor*>(pd_new(ofeliaSetColor_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaSetColor::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("r"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("g"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("b"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("a"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetColor_bang(t_ofeliaSetColor *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        ofGetCurrentRenderer()->setColor(x->color.r, x->color.g, x->color.b, x->color.a);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetColor::objName);
    }
}

void ofeliaSetColor_r(t_ofeliaSetColor *x, t_floatarg f)
{
    x->color.r = static_cast<unsigned char>(f);
}

void ofeliaSetColor_g(t_ofeliaSetColor *x, t_floatarg f)
{
    x->color.g = static_cast<unsigned char>(f);
}

void ofeliaSetColor_b(t_ofeliaSetColor *x, t_floatarg f)
{
    x->color.b = static_cast<unsigned char>(f);
}

void ofeliaSetColor_a(t_ofeliaSetColor *x, t_floatarg f)
{
    x->color.a = static_cast<unsigned char>(f);
}

void ofeliaSetColor_set(t_ofeliaSetColor *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaSetColor::objName);
}

void ofeliaSetColor_print(t_ofeliaSetColor *x)
{
    post("\n[%s]", t_ofeliaSetColor::objName);
    post("r : %d", x->color.r);
    post("g : %d", x->color.g);
    post("b : %d", x->color.b);
    post("a : %d", x->color.a);
}

void ofeliaSetColor_setup()
{
    ofeliaSetColor_class = class_new(gensym("ofSetColor"),
                                     reinterpret_cast<t_newmethod>(ofeliaSetColor_new),
                                     0, sizeof(t_ofeliaSetColor),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_bang));
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_r),
                    gensym("r"), A_FLOAT, 0);
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_g),
                    gensym("g"), A_FLOAT, 0);
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_b),
                    gensym("b"), A_FLOAT, 0);
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_a),
                    gensym("a"), A_FLOAT, 0);
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetColor_class, reinterpret_cast<t_method>(ofeliaSetColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetBgColor object methods
 */
void *ofeliaSetBgColor_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetBgColor *x = reinterpret_cast<t_ofeliaSetBgColor*>(pd_new(ofeliaSetBgColor_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getColorFromArgs(argc, argv, x->bgColor, t_ofeliaSetBgColor::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("r"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("g"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("b"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetBgColor_bang(t_ofeliaSetBgColor *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setBackgroundColor(x->bgColor);
        value_setfloat(t_ofeliaGetBgColorR::getBgColorRSym, static_cast<t_float>(x->bgColor.r));
        value_setfloat(t_ofeliaGetBgColorG::getBgColorGSym, static_cast<t_float>(x->bgColor.g));
        value_setfloat(t_ofeliaGetBgColorB::getBgColorBSym, static_cast<t_float>(x->bgColor.b));
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetBgColor::objName);
    }
}

void ofeliaSetBgColor_r(t_ofeliaSetBgColor *x, t_floatarg f)
{
    x->bgColor.r = static_cast<unsigned char>(f);
}

void ofeliaSetBgColor_g(t_ofeliaSetBgColor *x, t_floatarg f)
{
    x->bgColor.g = static_cast<unsigned char>(f);
}

void ofeliaSetBgColor_b(t_ofeliaSetBgColor *x, t_floatarg f)
{
    x->bgColor.b = static_cast<unsigned char>(f);
}

void ofeliaSetBgColor_set(t_ofeliaSetBgColor *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->bgColor, t_ofeliaSetBgColor::objName);
}

void ofeliaSetBgColor_print(t_ofeliaSetBgColor *x)
{
    post("\n[%s]", t_ofeliaSetBgColor::objName);
    post("r : %d", x->bgColor.r);
    post("g : %d", x->bgColor.g);
    post("b : %d", x->bgColor.b);
}

void ofeliaSetBgColor_setup()
{
    ofeliaSetBgColor_class = class_new(gensym("ofSetBgColor"),
                                       reinterpret_cast<t_newmethod>(ofeliaSetBgColor_new),
                                       0, sizeof(t_ofeliaSetBgColor),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_bang));
    class_addmethod(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_r),
                    gensym("r"), A_FLOAT, 0);
    class_addmethod(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_g),
                    gensym("g"), A_FLOAT, 0);
    class_addmethod(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_b),
                    gensym("b"), A_FLOAT, 0);
    class_addmethod(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetBgColor_class, reinterpret_cast<t_method>(ofeliaSetBgColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetRectMode object methods
 */
void *ofeliaSetRectMode_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetRectMode *x = reinterpret_cast<t_ofeliaSetRectMode*>(pd_new(ofeliaSetRectMode_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaSetRectMode::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("rectMode"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetRectMode_bang(t_ofeliaSetRectMode *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setRectMode(x->rectMode);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetRectMode::objName);
    }
}

void ofeliaSetRectMode_set(t_ofeliaSetRectMode *x, t_symbol *s, int argc, t_atom *argv)
{
    getRectModeFromArgs(argc, argv, x->rectMode, t_ofeliaSetRectMode::objName);
}

void ofeliaSetRectMode_print(t_ofeliaSetRectMode *x)
{
    post("\n[%s]", t_ofeliaSetRectMode::objName);
    t_string mode;
    
    if (x->rectMode == OF_RECTMODE_CORNER)
        mode = "CORNER";
    else if (x->rectMode == OF_RECTMODE_CENTER)
        mode = "CENTER";
    post("rectMode : %s", mode.c_str());
}

void ofeliaSetRectMode_setup()
{
    ofeliaSetRectMode_class = class_new(gensym("ofSetRectMode"),
                                        reinterpret_cast<t_newmethod>(ofeliaSetRectMode_new),
                                        0, sizeof(t_ofeliaSetRectMode),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetRectMode_class, reinterpret_cast<t_method>(ofeliaSetRectMode_bang));
    class_addmethod(ofeliaSetRectMode_class, reinterpret_cast<t_method>(ofeliaSetRectMode_set),
                    gensym("rectMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetRectMode_class, reinterpret_cast<t_method>(ofeliaSetRectMode_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetRectMode_class, reinterpret_cast<t_method>(ofeliaSetRectMode_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetTextMode object methods
 */
void *ofeliaSetTextMode_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetTextMode *x = reinterpret_cast<t_ofeliaSetTextMode*>(pd_new(ofeliaSetTextMode_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getTextModeFromArgs(argc, argv, x->textMode, t_ofeliaSetTextMode::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("horMode"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("verMode"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetTextMode_bang(t_ofeliaSetTextMode *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        t_ofeliaSetTextMode::textModeVec.back() = x->textMode;
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetTextMode::objName);
    }
}

void ofeliaSetTextMode_horMode(t_ofeliaSetTextMode *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 1) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            const int horMode = static_cast<int>(argv[0].a_w.w_float);
            
            if (horMode < 0 || horMode > 2) {
                
                error("%s: horMode out of range", t_ofeliaSetTextMode::objName);
                return;
            }
            x->textMode.horMode = static_cast<TextAlignHorMode>(horMode);
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *horMode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(horMode, "LEFT"))
                x->textMode.horMode = TEXT_ALIGN_LEFT;
            else if (!strcmp(horMode, "CENTER"))
                x->textMode.horMode = TEXT_ALIGN_CENTER;
            else if (!strcmp(horMode, "RIGHT"))
                x->textMode.horMode = TEXT_ALIGN_RIGHT;
            else
                error("%s: no method for '%s'", t_ofeliaSetTextMode::objName, horMode);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSetTextMode::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaSetTextMode::objName);
    }
}

void ofeliaSetTextMode_verMode(t_ofeliaSetTextMode *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 1) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            const int verMode = static_cast<int>(argv[0].a_w.w_float);
            
            if (verMode < 0 || verMode > 2) {
                
                error("%s: verMode out of range", t_ofeliaSetTextMode::objName);
                return;
            }
            x->textMode.verMode = static_cast<TextAlignVerMode>(verMode);
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *verMode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(verMode, "TOP"))
                x->textMode.verMode = TEXT_ALIGN_TOP;
            else if (!strcmp(verMode, "MIDDLE"))
                x->textMode.verMode = TEXT_ALIGN_MIDDLE;
            else if (!strcmp(verMode, "BOTTOM"))
                x->textMode.verMode = TEXT_ALIGN_BOTTOM;
            else
                error("%s: no method for '%s'", t_ofeliaSetTextMode::objName, verMode);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSetTextMode::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaSetTextMode::objName);
    }
}

void ofeliaSetTextMode_set(t_ofeliaSetTextMode *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextModeFromArgs(argc, argv, x->textMode, t_ofeliaSetTextMode::objName);
}

void ofeliaSetTextMode_print(t_ofeliaSetTextMode *x)
{
    post("\n[%s]", t_ofeliaSetTextMode::objName);
    t_string horMode, verMode;
    
    switch (x->textMode.horMode) {
            
        case TEXT_ALIGN_LEFT:
            horMode = "LEFT";
            break;
        case TEXT_ALIGN_CENTER:
            horMode = "CENTER";
            break;
        case TEXT_ALIGN_RIGHT:
            horMode = "RIGHT";
            break;
        default:
            break;
    }
    post("horMode : %s", horMode.c_str());
    
    switch (x->textMode.verMode) {
            
        case TEXT_ALIGN_TOP:
            verMode = "TOP";
            break;
        case TEXT_ALIGN_MIDDLE:
            verMode = "MIDDLE";
            break;
        case TEXT_ALIGN_BOTTOM:
            verMode = "BOTTOM";
            break;
        default:
            break;
    }
    post("verMode : %s", verMode.c_str());
}

void ofeliaSetTextMode_setup()
{
    ofeliaSetTextMode_class = class_new(gensym("ofSetTextMode"),
                                        reinterpret_cast<t_newmethod>(ofeliaSetTextMode_new),
                                        0, sizeof(t_ofeliaSetTextMode),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetTextMode_class, reinterpret_cast<t_method>(ofeliaSetTextMode_bang));
    class_addmethod(ofeliaSetTextMode_class, reinterpret_cast<t_method>(ofeliaSetTextMode_horMode),
                    gensym("horMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetTextMode_class, reinterpret_cast<t_method>(ofeliaSetTextMode_verMode),
                    gensym("verMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetTextMode_class, reinterpret_cast<t_method>(ofeliaSetTextMode_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetTextMode_class, reinterpret_cast<t_method>(ofeliaSetTextMode_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetFillMode object methods
 */
bool getFillModeFromArgs(const int argc, const t_atom *argv, ofFillFlag &fillMode) {
    
    switch (argc) {
            
        case 0:
        {
            fillMode = OF_FILLED;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                fillMode = static_cast<ofFillFlag>(argv[0].a_w.w_float != 0.0f);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "OUTLINE"))
                    fillMode = OF_OUTLINE;
                else if (!strcmp(mode, "FILLED"))
                    fillMode = OF_FILLED;
                else {
                    
                    error("%s: no method for '%s'", t_ofeliaSetFillMode::objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetFillMode::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetFillMode::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetFillMode_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetFillMode *x = reinterpret_cast<t_ofeliaSetFillMode*>(pd_new(ofeliaSetFillMode_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getFillModeFromArgs(argc, argv, x->fillMode))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("fillMode"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetFillMode_bang(t_ofeliaSetFillMode *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setFillMode(x->fillMode);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetFillMode::objName);
    }
}

void ofeliaSetFillMode_set(t_ofeliaSetFillMode *x, t_symbol *s, int argc, t_atom *argv)
{
    getFillModeFromArgs(argc, argv, x->fillMode);
}

void ofeliaSetFillMode_print(t_ofeliaSetFillMode *x)
{
    post("\n[%s]", t_ofeliaSetFillMode::objName);
    t_string mode;
    
    if (x->fillMode == OF_OUTLINE)
        mode = "OUTLINE";
    else if (x->fillMode == OF_FILLED)
        mode = "FILLED";
    post("fillMode : %s", mode.c_str());
}

void ofeliaSetFillMode_setup()
{
    ofeliaSetFillMode_class = class_new(gensym("ofSetFillMode"),
                                        reinterpret_cast<t_newmethod>(ofeliaSetFillMode_new),
                                        0, sizeof(t_ofeliaSetFillMode),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetFillMode_class, reinterpret_cast<t_method>(ofeliaSetFillMode_bang));
    class_addmethod(ofeliaSetFillMode_class, reinterpret_cast<t_method>(ofeliaSetFillMode_set),
                    gensym("fillMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetFillMode_class, reinterpret_cast<t_method>(ofeliaSetFillMode_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetFillMode_class, reinterpret_cast<t_method>(ofeliaSetFillMode_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetPolyMode object methods
 */
bool getPolyModeFromArgs(const int argc, const t_atom *argv, ofPolyWindingMode &polyMode) {
    
    switch (argc) {
            
        case 0:
        {
            polyMode = OF_POLY_WINDING_ODD;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int mode = static_cast<int>(argv[0].a_w.w_float);
                
                if (mode < 0 || mode > 4) {
                    
                    error("%s: polyMode out of range", t_ofeliaSetPolyMode::objName);
                    return 0;
                }
                polyMode = static_cast<ofPolyWindingMode>(mode);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "ODD"))
                    polyMode = OF_POLY_WINDING_ODD;
                else if (!strcmp(mode, "NONZERO"))
                    polyMode = OF_POLY_WINDING_NONZERO;
                else if (!strcmp(mode, "POSITIVE"))
                    polyMode = OF_POLY_WINDING_POSITIVE;
                else if (!strcmp(mode, "NEGATIVE"))
                    polyMode = OF_POLY_WINDING_NEGATIVE;
                else if (!strcmp(mode, "ABS_GEQ_TWO"))
                    polyMode = OF_POLY_WINDING_ABS_GEQ_TWO;
                else {
                    
                    error("%s: no method for '%s'", t_ofeliaSetPolyMode::objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetPolyMode::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetPolyMode::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetPolyMode_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetPolyMode *x = reinterpret_cast<t_ofeliaSetPolyMode*>(pd_new(ofeliaSetPolyMode_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getPolyModeFromArgs(argc, argv, x->polyMode))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("polyMode"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetPolyMode_bang(t_ofeliaSetPolyMode *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setPolyMode(x->polyMode);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetPolyMode::objName);
    }
}

void ofeliaSetPolyMode_set(t_ofeliaSetPolyMode *x, t_symbol *s, int argc, t_atom *argv)
{
    getPolyModeFromArgs(argc, argv, x->polyMode);
}

void ofeliaSetPolyMode_print(t_ofeliaSetPolyMode *x)
{
    post("\n[%s]", t_ofeliaSetPolyMode::objName);
    t_string mode;
    
    switch (x->polyMode) {
            
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

void ofeliaSetPolyMode_setup()
{
    ofeliaSetPolyMode_class = class_new(gensym("ofSetPolyMode"),
                                        reinterpret_cast<t_newmethod>(ofeliaSetPolyMode_new),
                                        0, sizeof(t_ofeliaSetPolyMode),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetPolyMode_class, reinterpret_cast<t_method>(ofeliaSetPolyMode_bang));
    class_addmethod(ofeliaSetPolyMode_class, reinterpret_cast<t_method>(ofeliaSetPolyMode_set),
                    gensym("polyMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetPolyMode_class, reinterpret_cast<t_method>(ofeliaSetPolyMode_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetPolyMode_class, reinterpret_cast<t_method>(ofeliaSetPolyMode_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetBlendMode object methods
 */
bool getBlendModeFromArgs(const int argc, const t_atom *argv, ofBlendMode &blendMode) {
    
    switch (argc) {
            
        case 0:
        {
            blendMode = OF_BLENDMODE_ALPHA;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int mode = static_cast<int>(argv[0].a_w.w_float);
                
                if (mode < 0 || mode > 5) {
                 
                    error("%s: blendMode out of range", t_ofeliaSetBlendMode::objName);
                    return 0;
                }
                blendMode = static_cast<ofBlendMode>(mode);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "OFF"))
                    blendMode = OF_BLENDMODE_DISABLED;
                else if (!strcmp(mode, "ALPHA"))
                    blendMode = OF_BLENDMODE_ALPHA;
                else if (!strcmp(mode, "ADD"))
                    blendMode = OF_BLENDMODE_ADD;
                else if (!strcmp(mode, "SUBTRACT"))
                    blendMode = OF_BLENDMODE_SUBTRACT;
                else if (!strcmp(mode, "MULTIPLY"))
                    blendMode = OF_BLENDMODE_MULTIPLY;
                else if (!strcmp(mode, "SCREEN"))
                    blendMode = OF_BLENDMODE_SCREEN;
                else {
                    
                    error("%s: no method for '%s'", t_ofeliaSetBlendMode::objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetBlendMode::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetBlendMode::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetBlendMode_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetBlendMode *x = reinterpret_cast<t_ofeliaSetBlendMode*>(pd_new(ofeliaSetBlendMode_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBlendModeFromArgs(argc, argv, x->blendMode))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("blendMode"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetBlendMode_bang(t_ofeliaSetBlendMode *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setBlendMode(x->blendMode);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetBlendMode::objName);
    }
}

void ofeliaSetBlendMode_set(t_ofeliaSetBlendMode *x, t_symbol *s, int argc, t_atom *argv)
{
    getBlendModeFromArgs(argc, argv, x->blendMode);
}

void ofeliaSetBlendMode_print(t_ofeliaSetBlendMode *x)
{
    post("\n[%s]", t_ofeliaSetBlendMode::objName);
    t_string mode;
    
    switch (x->blendMode) {
            
        case OF_BLENDMODE_DISABLED:
            mode = "OFF";
            break;
        case OF_BLENDMODE_ALPHA:
            mode = "ALPHA";
            break;
        case OF_BLENDMODE_ADD:
            mode = "ADD";
            break;
        case OF_BLENDMODE_SUBTRACT:
            mode = "SUBTRACT";
            break;
        case OF_BLENDMODE_MULTIPLY:
            mode = "MULTIPLY";
            break;
        case OF_BLENDMODE_SCREEN:
            mode = "SCREEN";
            break;
        default:
            break;
    }
    post("blendMode : %s", mode.c_str());
}

void ofeliaSetBlendMode_setup()
{
    ofeliaSetBlendMode_class = class_new(gensym("ofSetBlendMode"),
                                         reinterpret_cast<t_newmethod>(ofeliaSetBlendMode_new),
                                         0, sizeof(t_ofeliaSetBlendMode),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetBlendMode_class, reinterpret_cast<t_method>(ofeliaSetBlendMode_bang));
    class_addmethod(ofeliaSetBlendMode_class, reinterpret_cast<t_method>(ofeliaSetBlendMode_set),
                    gensym("blendMode"), A_GIMME, 0);
    class_addmethod(ofeliaSetBlendMode_class, reinterpret_cast<t_method>(ofeliaSetBlendMode_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetBlendMode_class, reinterpret_cast<t_method>(ofeliaSetBlendMode_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetLineWidth object methods
 */
bool getWidthFromArgs(const int argc, const t_atom *argv, float &width) {
    
    switch (argc) {
            
        case 0:
        {
            width = 1.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                float w = argv[0].a_w.w_float;
                
                if (w <= 0.0f) {
                    
                    error("%s: width out of range", t_ofeliaSetLineWidth::objName);
                    return 0;
                }
                width = w;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetLineWidth::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetLineWidth::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetLineWidth_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetLineWidth *x = reinterpret_cast<t_ofeliaSetLineWidth*>(pd_new(ofeliaSetLineWidth_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getWidthFromArgs(argc, argv, x->lineWidth))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("lineWidth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetLineWidth_bang(t_ofeliaSetLineWidth *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setLineWidth(x->lineWidth * ofeliaWindow::scaleFactor);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetLineWidth::objName);
    }
}

void ofeliaSetLineWidth_set(t_ofeliaSetLineWidth *x, t_symbol *s, int argc, t_atom *argv)
{
    getWidthFromArgs(argc, argv, x->lineWidth);
}

void ofeliaSetLineWidth_print(t_ofeliaSetLineWidth *x)
{
    post("\n[%s]", t_ofeliaSetLineWidth::objName);
    post("lineWidth : %g", x->lineWidth);
}

void ofeliaSetLineWidth_setup()
{
    ofeliaSetLineWidth_class = class_new(gensym("ofSetLineWidth"),
                                         reinterpret_cast<t_newmethod>(ofeliaSetLineWidth_new),
                                         0, sizeof(t_ofeliaSetLineWidth),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetLineWidth_class, reinterpret_cast<t_method>(ofeliaSetLineWidth_bang));
    class_addmethod(ofeliaSetLineWidth_class, reinterpret_cast<t_method>(ofeliaSetLineWidth_set),
                    gensym("lineWidth"), A_GIMME, 0);
    class_addmethod(ofeliaSetLineWidth_class, reinterpret_cast<t_method>(ofeliaSetLineWidth_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetLineWidth_class, reinterpret_cast<t_method>(ofeliaSetLineWidth_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetLineSmoothing object methods
 */
void *ofeliaSetLineSmoothing_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetLineSmoothing *x = reinterpret_cast<t_ofeliaSetLineSmoothing*>(pd_new(ofeliaSetLineSmoothing_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->lineSmoothing, t_ofeliaSetLineSmoothing::objName))
            return 0;
    }
    else {
        
        x->lineSmoothing = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("lineSmoothing"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetLineSmoothing_bang(t_ofeliaSetLineSmoothing *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setLineSmoothing(x->lineSmoothing);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetLineSmoothing::objName);
    }
}

void ofeliaSetLineSmoothing_set(t_ofeliaSetLineSmoothing *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->lineSmoothing, t_ofeliaSetLineSmoothing::objName);
}

void ofeliaSetLineSmoothing_print(t_ofeliaSetLineSmoothing *x)
{
    post("\n[%s]", t_ofeliaSetLineSmoothing::objName);
    printToggle("lineSmoothing", x->lineSmoothing);
}

void ofeliaSetLineSmoothing_setup()
{
    ofeliaSetLineSmoothing_class = class_new(gensym("ofSetLineSmoothing"),
                                         reinterpret_cast<t_newmethod>(ofeliaSetLineSmoothing_new),
                                         0, sizeof(t_ofeliaSetLineSmoothing),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetLineSmoothing_class, reinterpret_cast<t_method>(ofeliaSetLineSmoothing_bang));
    class_addmethod(ofeliaSetLineSmoothing_class, reinterpret_cast<t_method>(ofeliaSetLineSmoothing_set),
                    gensym("lineSmoothing"), A_GIMME, 0);
    class_addmethod(ofeliaSetLineSmoothing_class, reinterpret_cast<t_method>(ofeliaSetLineSmoothing_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetLineSmoothing_class, reinterpret_cast<t_method>(ofeliaSetLineSmoothing_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetCircleRes object methods
 */
bool getCircleResFromArgs(const int argc, const t_atom *argv, int &circleRes) {
    
    switch (argc) {
            
        case 0:
        {
            circleRes = OFELIA_DEFAULT_RESOLUTION;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int res = static_cast<int>(argv[0].a_w.w_float);
                
                if (res < 1) {
                    
                    error("%s: circleRes out of range", t_ofeliaSetCircleRes::objName);
                    return 0;
                }
                circleRes = res;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetCircleRes::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetCircleRes::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetCircleRes_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetCircleRes *x = reinterpret_cast<t_ofeliaSetCircleRes*>(pd_new(ofeliaSetCircleRes_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCircleResFromArgs(argc, argv, x->circleRes))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("circleRes"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetCircleRes_bang(t_ofeliaSetCircleRes *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        ofGetCurrentRenderer()->setCircleResolution(x->circleRes);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetCircleRes::objName);
    }
}

void ofeliaSetCircleRes_set(t_ofeliaSetCircleRes *x, t_symbol *s, int argc, t_atom *argv)
{
    getCircleResFromArgs(argc, argv, x->circleRes);
}

void ofeliaSetCircleRes_print(t_ofeliaSetCircleRes *x)
{
    post("\n[%s]", t_ofeliaSetCircleRes::objName);
    post("circleRes : %d", x->circleRes);
}

void ofeliaSetCircleRes_setup()
{
    ofeliaSetCircleRes_class = class_new(gensym("ofSetCircleRes"),
                                         reinterpret_cast<t_newmethod>(ofeliaSetCircleRes_new),
                                         0, sizeof(t_ofeliaSetCircleRes),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetCircleRes_class, reinterpret_cast<t_method>(ofeliaSetCircleRes_bang));
    class_addmethod(ofeliaSetCircleRes_class, reinterpret_cast<t_method>(ofeliaSetCircleRes_set),
                    gensym("circleRes"), A_GIMME, 0);
    class_addmethod(ofeliaSetCircleRes_class, reinterpret_cast<t_method>(ofeliaSetCircleRes_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetCircleRes_class, reinterpret_cast<t_method>(ofeliaSetCircleRes_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetCurveRes object methods
 */
bool getCurveResFromArgs(const int argc, const t_atom *argv, int &curveRes) {
    
    switch (argc) {
            
        case 0:
        {
            curveRes = OFELIA_DEFAULT_RESOLUTION;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int res = static_cast<int>(argv[0].a_w.w_float);
                
                if (res < 1) {
                    
                    error("%s: curveRes out of range", t_ofeliaSetCurveRes::objName);
                    return 0;
                }
                curveRes = res;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetCurveRes::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetCurveRes::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetCurveRes_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetCurveRes *x = reinterpret_cast<t_ofeliaSetCurveRes*>(pd_new(ofeliaSetCurveRes_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCurveResFromArgs(argc, argv, x->curveRes))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("curveRes"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetCurveRes_bang(t_ofeliaSetCurveRes *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setCurveResolution(x->curveRes);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetCurveRes::objName);
    }
}

void ofeliaSetCurveRes_set(t_ofeliaSetCurveRes *x, t_symbol *s, int argc, t_atom *argv)
{
    getCurveResFromArgs(argc, argv, x->curveRes);
}

void ofeliaSetCurveRes_print(t_ofeliaSetCurveRes *x)
{
    post("\n[%s]", t_ofeliaSetCurveRes::objName);
    post("curveRes : %d", x->curveRes);
}

void ofeliaSetCurveRes_setup()
{
    ofeliaSetCurveRes_class = class_new(gensym("ofSetCurveRes"),
                                        reinterpret_cast<t_newmethod>(ofeliaSetCurveRes_new),
                                        0, sizeof(t_ofeliaSetCurveRes),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetCurveRes_class, reinterpret_cast<t_method>(ofeliaSetCurveRes_bang));
    class_addmethod(ofeliaSetCurveRes_class, reinterpret_cast<t_method>(ofeliaSetCurveRes_set),
                    gensym("curveRes"), A_GIMME, 0);
    class_addmethod(ofeliaSetCurveRes_class, reinterpret_cast<t_method>(ofeliaSetCurveRes_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetCurveRes_class, reinterpret_cast<t_method>(ofeliaSetCurveRes_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaStyle_setup()
{
    ofeliaPushStyle_setup();
    ofeliaPopStyle_setup();
    ofeliaSetColor_setup();
    ofeliaSetBgColor_setup();
    ofeliaSetRectMode_setup();
    ofeliaSetTextMode_setup();
    ofeliaSetFillMode_setup();
    ofeliaSetPolyMode_setup();
    ofeliaSetBlendMode_setup();
    ofeliaSetLineWidth_setup();
    ofeliaSetLineSmoothing_setup();
    ofeliaSetCircleRes_setup();
    ofeliaSetCurveRes_setup();
}

