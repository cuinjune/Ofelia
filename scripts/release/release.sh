#!/bin/bash
# this script is for creating zip archives for release

# create directory and copy macOS related sources into it
macOSPath="ofxOfelia_macOS"
rm -rf "${macOSPath}"
mkdir "${macOSPath}"
mkdir "${macOSPath}/scripts"
cp -rp "../../scripts/makeAliases" "${macOSPath}/scripts"
cp -rp "../../macOSExample" "${macOSPath}"
cp -rp "../../macOSExternal" "${macOSPath}"
cp -rp "../../libs" "${macOSPath}"
cp -rp "../../src" "${macOSPath}"
cp -rp "../../LICENSE.txt" "${macOSPath}"
cp -rp "../../CHANGES.txt" "${macOSPath}"
cp -rp "../../README.md" "${macOSPath}"
cp -rp "../../addon_config.mk" "${macOSPath}"

# create directory and copy Linux related sources into it
LinuxPath="ofxOfelia_Linux"
rm -rf "${LinuxPath}"
mkdir "${LinuxPath}"
mkdir "${LinuxPath}/scripts"
cp -rp "../../scripts/Linux64" "${LinuxPath}/scripts"
cp -rp "../../scripts/LinuxArmv6" "${LinuxPath}/scripts"
cp -rp "../../scripts/makeAliases" "${LinuxPath}/scripts"
cp -rp "../../LinuxExample" "${LinuxPath}"
cp -rp "../../LinuxExternal" "${LinuxPath}"
cp -rp "../../libs" "${LinuxPath}"
cp -rp "../../src" "${LinuxPath}"
cp -rp "../../LICENSE.txt" "${LinuxPath}"
cp -rp "../../CHANGES.txt" "${LinuxPath}"
cp -rp "../../README.md" "${LinuxPath}"
cp -rp "../../addon_config.mk" "${LinuxPath}"

# create directory and copy Windows related sources into it
WindowsPath="ofxOfelia_Windows"
rm -rf "${WindowsPath}"
mkdir "${WindowsPath}"
mkdir "${WindowsPath}/scripts"
cp -rp "../../scripts/makeAliases" "${WindowsPath}/scripts"
cp -rp "../../WindowsExample" "${WindowsPath}"
cp -rp "../../WindowsExternal" "${WindowsPath}"
cp -rp "../../libs" "${WindowsPath}"
cp -rp "../../src" "${WindowsPath}"
cp -rp "../../LICENSE.txt" "${WindowsPath}"
cp -rp "../../CHANGES.txt" "${WindowsPath}"
cp -rp "../../README.md" "${WindowsPath}"
cp -rp "../../addon_config.mk" "${WindowsPath}"

# create directory and copy iOS related sources into it
iOSPath="ofxOfelia_iOS"
rm -rf "${iOSPath}"
mkdir "${iOSPath}"
mkdir "${iOSPath}/scripts"
cp -rp "../../scripts/makeAliases" "${iOSPath}/scripts"
cp -rp "../../iOSExample" "${iOSPath}"
cp -rp "../../libs" "${iOSPath}"
cp -rp "../../src" "${iOSPath}"
cp -rp "../../LICENSE.txt" "${iOSPath}"
cp -rp "../../CHANGES.txt" "${iOSPath}"
cp -rp "../../README.md" "${iOSPath}"
cp -rp "../../addon_config.mk" "${iOSPath}"

# create directory and copy Android related sources into it
# AndroidPath="ofxOfelia_Android"
# rm -rf "${AndroidPath}"
# mkdir "${AndroidPath}"
# mkdir "${AndroidPath}/scripts"
# cp -rp "../../scripts/makeAliases" "${AndroidPath}/scripts"
# cp -rp "../../AndroidExample" "${AndroidPath}"
# cp -rp "../../libs" "${AndroidPath}"
# cp -rp "../../src" "${AndroidPath}"
# cp -rp "../../LICENSE.txt" "${AndroidPath}"
# cp -rp "../../CHANGES.txt" "${AndroidPath}"
# cp -rp "../../README.md" "${AndroidPath}"
# cp -rp "../../addon_config.mk" "${AndroidPath}"

# delete all .DS_Store files
find . -name '.DS_Store' -type f -delete

# zip each created directory
zip -r ofxOfelia_macOS.zip ofxOfelia_macOS
zip -r ofxOfelia_Linux.zip ofxOfelia_Linux
zip -r ofxOfelia_Windows.zip ofxOfelia_Windows
zip -r ofxOfelia_iOS.zip ofxOfelia_iOS
# zip -r ofxOfelia_Android.zip ofxOfelia_Android

# remove all created directories
rm -rf "${macOSPath}"
rm -rf "${LinuxPath}"
rm -rf "${WindowsPath}"
rm -rf "${iOSPath}"
# rm -rf "${AndroidPath}"
