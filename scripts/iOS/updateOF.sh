#!/bin/bash

# path to openFrameworks base folder
ofPath=`cd "../../../.."; pwd`

# paths to be updated
paths=(
"${ofPath}/addons/ofxiOS/src/app/ofAppiOSWindow.h"
"${ofPath}/addons/ofxiOS/src/app/ofAppiOSWindow.mm"
"${ofPath}/addons/ofxiOS/src/core/ofxiOSAppDelegate.mm"
"${ofPath}/addons/ofxiOS/src/core/ofxiOSViewController.mm"
"${ofPath}/libs/openFrameworks/gl/ofTexture.cpp"
"${ofPath}/libs/openFrameworks/graphics/ofTrueTypeFont.cpp"
)

# number of paths
numPaths="${#paths[@]}"

# update paths
for ((i=0; i<"${numPaths}"; i++));

do
path="${paths[$i]}"
srcPath=$(basename "${path}")
destPath=$(dirname "${path}")

# check if the update is successful or not
if yes | cp -rp "updateOF/${srcPath}" "${destPath}"; then
    echo "Updating: ${path}"
else
    echo "Failed updating: ${path}, exit status $?"
    exit 1
fi
done

echo "The update has been completed successfully"
