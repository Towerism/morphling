#!/bin/bash
set -ev

if [ "${COMPONENT}" == "game-server" ]; then
    export CXX=g++-5
    cd game-server
    mkdir -p build && cd build
    cmake ..
    make
fi
