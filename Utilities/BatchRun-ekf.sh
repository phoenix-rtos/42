#!/usr/bin/env bash
# This script submits multiple runs to run concurrently.
# OSX distributes the load among the available CPU cores.
# While untested, Linux might do something similar.
# Usage:  ./BatchRun.sh InOut1 InOut2 InOut3
# From Matlab or octave:
# system('./BatchRun.sh InOut1 InOut2 InOut3')
# The folders InOut1, InOut2, etc must already exist

./42 $1 ./Model & #> ./MonteCarlo/"$1"_log.txt 2>&1 &

sleep 1

path=$(pwd)
cd /home/lukasz/cubesat/phoenix-cubesat-project/_fs/host-generic-cubesat/root/ && ./usr/bin/satdevekf
cd $path

wait
echo "Run complete"
cd MonteCarlo
