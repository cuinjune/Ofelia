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

#include "ofeliaImageLoader.h"

ofeliaImageLoader::ofeliaImageLoader()
{
	nextID = 0;
    startThread();
    lastUpdate = 0;
}

ofeliaImageLoader::~ofeliaImageLoader()
{
	images_to_load_from_disk.close();
	images_to_update.close();
	waitForThread(true);
}

void ofeliaImageLoader::loadFromDisk(ofImage &image, t_string filename)
{
	nextID++;
	ofImageLoaderEntry entry(image);
	entry.filename = filename;
	entry.image->setUseTexture(false);
	images_to_load_from_disk.send(entry);
}

void ofeliaImageLoader::threadedFunction()
{
	ofImageLoaderEntry entry;

	while (images_to_load_from_disk.receive(entry)) {
        
        if(entry.image->load(entry.filename.c_str()))
            images_to_update.send(entry);
	}
}

void ofeliaImageLoader::update()
{
	ofImageLoaderEntry entry;
    
	if (images_to_update.tryReceive(entry)) {
        
		entry.image->setUseTexture(true);
		entry.image->update();
	}
}

