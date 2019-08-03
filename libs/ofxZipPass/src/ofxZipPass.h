/*
 * Copyright (c) 2014 Antoine Rousseau <antoine@metalu.net>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *

 * This project uses zlib and minizip, copyrighted by Jean-loup Gailly, Mark Adler, Gilles Vollant
 * and others. Here is the license of zlib/minizip :

          ---------------------------------------------------------------------------------

        Condition of use and distribution are the same than zlib :

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  ---------------------------------------------------------------------------------
 */
 
#pragma once
#include "unzip.h"
#include <string>

class ofxUnzipPass {
	public :
	ofxUnzipPass(const std::string& inzfile, const std::string& inpass = "");
	~ofxUnzipPass();
	
	bool unzipTo(const std::string& outPath);
	bool isOk();
	
	private :
	unzFile file;
	unz_global_info  globalInfo;
	std::string password;
};

