#!/bin/bash

# path to openFrameworks base folder
ofPath=`cd "../../../.."; pwd`

# path to be updated
path="${ofPath}/addons/ofxOfelia/macOSExternal/src/ofeliaAliases.cpp"

# compile the code
clang++ -Wall -std=c++14 makeAliases/makeAliases.cpp -o makeAliases/makeAliases

# run the binary (generates "ofeliaAliases.cpp")
./makeAliases/makeAliases

# remove the binary
rm -f makeAliases/makeAliases

# copy the generated cpp file to the destination path
srcPath=$(basename "${path}")
destPath=$(dirname "${path}")

# check if the update is successful or not
if yes | cp -rp "${srcPath}" "${destPath}"; then
echo "Updating: ${path}"
else
echo "Failed updating: ${path}, exit status $?"
exit 1
fi

echo "The update has been completed successfully"

# remove the original "ofeliaAliases.cpp"
rm -f "${srcPath}"
