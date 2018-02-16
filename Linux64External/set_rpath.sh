#!/bin/bash

patchelf --set-rpath '$ORIGIN/libs' bin/ofelia.*
