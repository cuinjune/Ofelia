#!/bin/bash

# path to openFrameworks base folder
ofPath=`cd "../../../.."; pwd`

# paths to be updated
paths=(
"${ofPath}/addons/ofxAndroid/ofAndroidLib/AndroidManifest.xml"
"${ofPath}/addons/ofxAndroid/ofAndroidLib/build.gradle"
"${ofPath}/addons/ofxAndroid/ofAndroidLib/project.properties"
"${ofPath}/addons/ofxAndroid/ofAndroidLib/src/cc/openframeworks/OFAndroid.java"
"${ofPath}/libs/openFrameworks/graphics/ofTrueTypeFont.cpp"
"${ofPath}/libs/openFrameworksCompiled/project/android/config.android.default.mk"
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
