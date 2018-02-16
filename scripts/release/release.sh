#!/bin/bash
# this script is for creating zip archives for release

# create directory and copy macOS related sources into it
macOSPath="ofxOfelia_macOS"
rm -rf "${macOSPath}"
mkdir "${macOSPath}"
mkdir "${macOSPath}/scripts"
cp -rp "../../scripts/macOS" "${macOSPath}/scripts"
cp -rp "../../macOSExample" "${macOSPath}"
cp -rp "../../macOSExternal" "${macOSPath}"
cp -rp "../../libs" "${macOSPath}"
cp -rp "../../LICENSE.txt" "${macOSPath}"
cp -rp "../../CHANGES.txt" "${macOSPath}"
cp -rp "../../README.md" "${macOSPath}"

# create directory and copy Linux related sources into it
LinuxPath="ofxOfelia_Linux"
rm -rf "${LinuxPath}"
mkdir "${LinuxPath}"
mkdir "${LinuxPath}/scripts"
cp -rp "../../scripts/Linux" "${LinuxPath}/scripts"
cp -rp "../../Linux64Example" "${LinuxPath}"
cp -rp "../../Linux64External" "${LinuxPath}"
cp -rp "../../libs" "${LinuxPath}"
cp -rp "../../LICENSE.txt" "${LinuxPath}"
cp -rp "../../CHANGES.txt" "${LinuxPath}"
cp -rp "../../README.md" "${LinuxPath}"

# create directory and copy Windows related sources into it
WinPath="ofxOfelia_Win"
rm -rf "${WinPath}"
mkdir "${WinPath}"
mkdir "${WinPath}/scripts"
cp -rp "../../scripts/Win" "${WinPath}/scripts"
cp -rp "../../Win32Example" "${WinPath}"
cp -rp "../../Win32External" "${WinPath}"
cp -rp "../../libs" "${WinPath}"
cp -rp "../../LICENSE.txt" "${WinPath}"
cp -rp "../../CHANGES.txt" "${WinPath}"
cp -rp "../../README.md" "${WinPath}"

# create directory and copy iOS related sources into it
iOSPath="ofxOfelia_iOS"
rm -rf "${iOSPath}"
mkdir "${iOSPath}"
mkdir "${iOSPath}/scripts"
cp -rp "../../scripts/iOS" "${iOSPath}/scripts"
cp -rp "../../iOSExample" "${iOSPath}"
cp -rp "../../iOSABExample" "${iOSPath}"
cp -rp "../../LICENSE.txt" "${iOSPath}"
cp -rp "../../CHANGES.txt" "${iOSPath}"
cp -rp "../../README.md" "${iOSPath}"

# create directory and copy Android related sources into it
AndroidPath="ofxOfelia_Android"
rm -rf "${AndroidPath}"
mkdir "${AndroidPath}"
mkdir "${AndroidPath}/scripts"
cp -rp "../../scripts/Android" "${AndroidPath}/scripts"
cp -rp "../../AndroidExample" "${AndroidPath}"
cp -rp "../../LICENSE.txt" "${AndroidPath}"
cp -rp "../../CHANGES.txt" "${AndroidPath}"
cp -rp "../../README.md" "${AndroidPath}"

# delete all .DS_Store files
find . -name '.DS_Store' -type f -delete

# zip each created directory
zip -r ofxOfelia_macOS.zip ofxOfelia_macOS
zip -r ofxOfelia_Linux.zip ofxOfelia_Linux
zip -r ofxOfelia_Win.zip ofxOfelia_Win
zip -r ofxOfelia_iOS.zip ofxOfelia_iOS
zip -r ofxOfelia_Android.zip ofxOfelia_Android

# remove all created directories
rm -rf "${macOSPath}"
rm -rf "${LinuxPath}"
rm -rf "${WinPath}"
rm -rf "${iOSPath}"
rm -rf "${AndroidPath}"


