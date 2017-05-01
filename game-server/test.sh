#!/bin/bash
for i in `seq 1 1`; do
    echo test one game $i
    ./bin/morphling-test-unit --gtest_filter=TicTacToePlayTest.TwoPlayersWhiteWin
    sleep 1s
done
