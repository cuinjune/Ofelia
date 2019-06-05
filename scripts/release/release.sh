#!/bin/bash
# this script is for creating zip archives for release

# create directory and copy macOS related sources into it
macOSPath="ofxOfelia_macOS"
rm -rf "${macOSPath}"
mkdir "${macOSPath}"
mkdir "${macOSPath}/scripts"
cp -rp "../../scripts/macOS" "${macOSPath}/scripts"
# cp -rp "../../macOSExample" "${macOSPath}"
cp -rp "../../macOSExternal" "${macOSPath}"
cp -rp "../../libs" "${macOSPath}"
cp -rp "../../LICENSE.txt" "${macOSPath}"
cp -rp "../../CHANGES.txt" "${macOSPath}"
cp -rp "../../README.md" "${macOSPath}"

# create directory and copy Linux64 related sources into it
Linux64Path="ofxOfelia_Linux64"
rm -rf "${Linux64Path}"
mkdir "${Linux64Path}"
mkdir "${Linux64Path}/scripts"
cp -rp "../../scripts/Linux64" "${Linux64Path}/scripts"
# cp -rp "../../Linux64Example" "${Linux64Path}"
cp -rp "../../Linux64External" "${Linux64Path}"
cp -rp "../../libs" "${Linux64Path}"
cp -rp "../../LICENSE.txt" "${Linux64Path}"
cp -rp "../../CHANGES.txt" "${Linux64Path}"
cp -rp "../../README.md" "${Linux64Path}"

# create directory and copy LinuxArmv6 related sources into it
LinuxArmv6Path="ofxOfelia_LinuxArmv6"
rm -rf "${LinuxArmv6Path}"
mkdir "${LinuxArmv6Path}"
mkdir "${LinuxArmv6Path}/scripts"
cp -rp "../../scripts/LinuxArmv6" "${LinuxArmv6Path}/scripts"
# cp -rp "../../LinuxArmv6Example" "${LinuxArmv6Path}"
cp -rp "../../LinuxArmv6External" "${LinuxArmv6Path}"
cp -rp "../../libs" "${LinuxArmv6Path}"
cp -rp "../../LICENSE.txt" "${LinuxArmv6Path}"
cp -rp "../../CHANGES.txt" "${LinuxArmv6Path}"
cp -rp "../../README.md" "${LinuxArmv6Path}"

# create directory and copy Windows related sources into it
WindowsPath="ofxOfelia_Windows"
rm -rf "${WindowsPath}"
mkdir "${WindowsPath}"
# mkdir "${WindowsPath}/scripts"
# cp -rp "../../scripts/Windows" "${WindowsPath}/scripts"
# cp -rp "../../WindowsExample" "${WindowsPath}"
cp -rp "../../WindowsExternal" "${WindowsPath}"
cp -rp "../../libs" "${WindowsPath}"
cp -rp "../../LICENSE.txt" "${WindowsPath}"
cp -rp "../../CHANGES.txt" "${WindowsPath}"
cp -rp "../../README.md" "${WindowsPath}"

# create directory and copy iOS related sources into it
# iOSPath="ofxOfelia_iOS"
# rm -rf "${iOSPath}"
# mkdir "${iOSPath}"
# mkdir "${iOSPath}/scripts"
# cp -rp "../../scripts/iOS" "${iOSPath}/scripts"
# cp -rp "../../iOSExample" "${iOSPath}"
# cp -rp "../../iOSABExample" "${iOSPath}"
# cp -rp "../../LICENSE.txt" "${iOSPath}"
# cp -rp "../../CHANGES.txt" "${iOSPath}"
# cp -rp "../../README.md" "${iOSPath}"

# create directory and copy Android related sources into it
# AndroidPath="ofxOfelia_Android"
# rm -rf "${AndroidPath}"
# mkdir "${AndroidPath}"
# mkdir "${AndroidPath}/scripts"
# cp -rp "../../scripts/Android" "${AndroidPath}/scripts"
# cp -rp "../../AndroidExample" "${AndroidPath}"
# cp -rp "../../LICENSE.txt" "${AndroidPath}"
# cp -rp "../../CHANGES.txt" "${AndroidPath}"
# cp -rp "../../README.md" "${AndroidPath}"

# delete all .DS_Store files
find . -name '.DS_Store' -type f -delete

# zip each created directory
zip -r ofxOfelia_macOS.zip ofxOfelia_macOS
zip -r ofxOfelia_Linux64.zip ofxOfelia_Linux64
zip -r ofxOfelia_LinuxArmv6.zip ofxOfelia_LinuxArmv6
zip -r ofxOfelia_Windows.zip ofxOfelia_Windows
# zip -r ofxOfelia_iOS.zip ofxOfelia_iOS
# zip -r ofxOfelia_Android.zip ofxOfelia_Android

# remove all created directories
rm -rf "${macOSPath}"
rm -rf "${Linux64Path}"
rm -rf "${LinuxArmv6Path}"
rm -rf "${WindowsPath}"
# rm -rf "${iOSPath}"
# rm -rf "${AndroidPath}"


