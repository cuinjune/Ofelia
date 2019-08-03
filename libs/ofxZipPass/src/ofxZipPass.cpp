
#include "ofxZipPass.h"
#include "ofLog.h"

using namespace std;


ofxUnzipPass::ofxUnzipPass(const string& inzfile, const std::string& inpass) : password(inpass) {
	file = unzOpen(inzfile.c_str());
	if(file != NULL) {
		if(unzGetGlobalInfo(file, &globalInfo) == UNZ_OK)
		{
			ofLogVerbose("ofxZipPass") << globalInfo.number_entry << " entries in the zip file";
		}
		else {
			unzClose(file);
			file = NULL;
		}
	}
}

bool ofxUnzipPass::isOk() {
	return (file != NULL);
}
	
ofxUnzipPass::~ofxUnzipPass() {
	if(file != NULL) unzClose(file);
}


bool ofxUnzipPass::unzipTo(const string& outPath) { 
	int ret = unzGoToFirstFile(file);
	unsigned char buffer[4096] = {0};

	if(! isOk()) return false;
	if(ret != UNZ_OK) return false;
	
	do{
		unz_file_info fileInfo ={0};
		ret = unzGetCurrentFileInfo(file, &fileInfo, NULL, 0, NULL, 0, NULL, 0);
		if(ret!=UNZ_OK)
		{
			return false;
		}
		
		char* filename = (char*) malloc( fileInfo.size_filename +1 );
		unzGetCurrentFileInfo(file, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
		filename[fileInfo.size_filename] = '\0';
		
		// check if it contains directory
		string fileStr = filename;
		free(filename);
		
		std::replace(fileStr.begin(), fileStr.end(), '\\', '/');
		
		bool isDirectory = false;
		
		if(*(fileStr.end()-1)=='/') isDirectory = true;
		
		string fullPath = outPath + "/" + fileStr;
		
		if(isDirectory)
			ofDirectory::createDirectory(fullPath, /*bool bRelativeToData=*/false, /*bool recursive=*/true);
		else {
			if(password.length() == 0) ret = unzOpenCurrentFile(file);
			else ret = unzOpenCurrentFilePassword(file, password.c_str());
		
			if(ret != UNZ_OK)
			{
				ofLogError("ofxZipPass") << "reading zip file" << (password.length() != 0?" with password: " + password:"") ;
				return false;
			}
			// reading data and write to file
			int read ;
			ofFile outFile = ofFile(fullPath, ofFile::WriteOnly, /*bool binary=*/true);
			outFile.create();
			
			FILE* fp = fopen(fullPath.c_str(), "wb");
			while(fp)
			{
				read = unzReadCurrentFile(file, buffer, 4096);
				if(read > 0)
				{
					fwrite(buffer, read, 1, fp);
				}
				else if(read < 0)
				{
					ofLogError("ofxZipPass") << "Failed to reading zip file";
					return false;
				}
				else break;				
			}
			if(fp) fclose(fp);
			unzCloseCurrentFile(file);
		}

		ret = unzGoToNextFile(file);
	} while(ret == UNZ_OK); // && ret != UNZ_END_OF_LIST_OF_FILE);
	return true;
}

