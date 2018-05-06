#!/bin/bash
cd ..


#NoJamming

for distance in {1..1500..100}
do
for interval in "0.01" "0.1" "0.2" "0.5" "0.8" "1" 
do
    #echo "scratch/${distance}-${interval}"
    #./waf --run "scratch/test_none --i"
    echo "distance:${distance}    interval:${interval}"
    ./waf --run "scratch/test_none --distanceToRx=${distance} --interval=${interval}" > /dev/null
    mv no.log "scratch/data/none-${distance}-${interval}.log"
done
done

