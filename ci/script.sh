#!/bin/bash
set -ev

if [ "${COMPONENT}" == "game-server" ]; then
    export CXX=g++
    cd game-server
    mkdir -p build && cd build
    cmake -DTEST=ON ..
    make build-tests
    make test ARGS=-VV
fi
