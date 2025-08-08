#!/bin/bash

socat PTY,link=/tmp/channel1A,raw,echo=0 PTY,link=/tmp/channel1B,raw,echo=0 &
pid1=$!
socat PTY,link=/tmp/channel2A,raw,echo=0 PTY,link=/tmp/channel2B,raw,echo=0 &
pid2=$!
./42 &
pid3=$!

trap "kill $pid1 $pid2 $pid3; exit" SIGINT

wait
