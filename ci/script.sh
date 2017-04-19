#!/bin/bash
set -ev

if [ "${COMPONENT}" == "game-server" ]; then
    export CXX=g++-6
    cd game-server
    mkdir -p build && cd build
    cmake -DTEST=ON ..
    make
    make build-tests
    make test ARGS=-VV
fi

if [ "${COMPONENT}" == "web-server" ]; then
    # build, and test web server here
    cd web-server
    make test
    make clean
fi
