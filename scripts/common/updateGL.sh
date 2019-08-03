#!/bin/bash

# compile the code
g++ -Wall -std=c++14 updateGL.cpp -o updateGL

# run the binary (updates "../../src/ofxOfeliaGL.cpp")
./updateGL

# remove the binary
rm -f updateGL
