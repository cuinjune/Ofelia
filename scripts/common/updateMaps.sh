#!/bin/bash

# compile the code
g++ -Wall -std=c++14 updateMaps.cpp -o updateMaps

# run the binary (updates "../../src/ofxOfeliaMaps.cpp")
# optionally use '-d' flag to log skipped functions and class methods
./updateMaps $1

# remove the binary
rm -f updateMaps
