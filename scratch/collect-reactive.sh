#!/bin/bash
cd ..

#ReactiveJamming

for disx in {0..15..1}
do
for disy in {0..10..1}
do
for interval in "0.01" "0.1" "1" 
do
    #echo "scratch/${distance}-${interval}"
    #./waf --run "scratch/test_none --i"
    echo "disx:${disx}    disy:${disy}    interval:${interval}"
    ./waf --run "scratch/test_reactive --disx=${disx} --disy=${disy} --interval=${interval}" > /dev/null
    mv reactive.log "scratch/data/reactive/reactive-${disx}-${disy}-${interval}.log"
done
done
done
