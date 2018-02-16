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

#include "ofeliaVersion.h"

void ofeliaPrintVersion()
{
    stringstream ss;
    ss << "------------------------------------------------------------" << '\n';
    ss << "ofelia" << ' ';
    ss << 'v';
    ss << OFELIA_MAJOR_VERSION << '.';
    ss << OFELIA_MINOR_VERSION << '.';
    ss << OFELIA_BUGFIX_VERSION << ':';
    ss << " compiled on ";
    ss << ofGetDay() << ' ';
    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    ss << months[ofGetMonth()-1] << ' ';
    ss << ofGetYear() << '\n';
    ss << "(c) 2018 Zack Lee <cuinjune@gmail.com>" << '\n';
    ss << "------------------------------------------------------------";
    post(ss.str().c_str());
}

