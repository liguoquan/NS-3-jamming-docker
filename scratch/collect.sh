#!/bin/bash
cd ..
for time in {1..1000}
do
    ./waf --run scratch/constant-jammer-example
    mv constant.log "scratch/data/constant-${time}.log"
done
