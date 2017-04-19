#!/bin/bash
set -ev

if [ "${COMPONENT}" == "game-server" ]; then
    # run any install commands necessary for game-server here
    cd game-server
fi

if [ "${COMPONENT}" == "web-server" ]; then
    # run any install commands necessary for web-server here
    # e.g. pip install -r requirements.txt
    cd web-server
    pip install -r requirements.txt
fi