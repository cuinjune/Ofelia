#!/bin/bash

# path to openFrameworks base folder
ofPath=`cd "../../../.."; pwd`

# paths to be updated
paths=(
"${ofPath}/addons/ofxOfelia/src/ofxOfeliaAliases.cpp"
)

# compile the code
clang++ -Wall -std=c++14 makeAliases.cpp -o makeAliases

# run the binary (generates "ofxOfeliaAliases.cpp")
./makeAliases

# remove the binary
rm -f makeAliases

# number of paths
numPaths="${#paths[@]}"

# update paths
for ((i=0; i<"${numPaths}"; i++));

do
path="${paths[$i]}"
srcPath=$(basename "${path}")
destPath=$(dirname "${path}")

# check if the update is successful or not
if yes | cp -rp "${srcPath}" "${destPath}"; then
    echo "Updating: ${path}"
else
    echo "Failed updating: ${path}, exit status $?"
    exit 1
fi
done

echo "The update has been completed successfully"

# remove the original "ofxOfeliaAliases.cpp"
rm -f "${srcPath}"
