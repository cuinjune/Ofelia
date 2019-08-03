#!/bin/bash

# compile the code
g++ -Wall -std=c++14 -I../../src updateHelpIntro.cpp ../../src/ofxOfeliaMaps.cpp -o updateHelpIntro

# run the binary (updates "../../ofelia/help-intro.pd")
./updateHelpIntro

# remove the binary
rm -f updateHelpIntro
