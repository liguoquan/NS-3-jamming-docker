#!/bin/bash
cd ..


#NoJamming

#for distance in {1..1500..100}
#do
#for interval in "0.01" "0.1" "0.2" "0.5" "0.8" "1" 
#do
    #echo "scratch/${distance}-${interval}"
    #./waf --run "scratch/test_none --i"
#    echo "distance:${distance}    interval:${interval}"
#    ./waf --run "scratch/test_none --distanceToRx=${distance} --interval=${interval}" > /dev/null
#    mv no.log "scratch/data/none-${distance}-${interval}.log"
#done
#done


#NoJamming

for SystemLoss in {1..12000..100}
do
for interval in "0.0005" "0.001" "0.005" "0.01" "0.1" "0.5" "1" 
do
    #echo "scratch/${distance}-${interval}"
    #./waf --run "scratch/test_none --i"
    echo "SystemLoss:${SystemLoss}    interval:${interval}"
    ./waf --run "scratch/test_none --SystemLoss=${SystemLoss} --interval=${interval}" > /dev/null
    mv no.log "scratch/data/none-${SystemLoss}-${interval}.log"
done
done

