#!/bin/bash
cd ..

#ConstantJamming

for x in {0..150..5}
do
for y in {0..100..5}
do
for interval in "0.01" "0.1" "1" 
do
    disx=$(echo $x | awk '{ printf "%0.2f\n" ,$1*0.1}')
    disy=$(echo $y | awk '{ printf "%0.2f\n" ,$1*0.1}')
    #echo "scratch/${distance}-${interval}"
    #./waf --run "scratch/test_none --i"
    echo "disx:${disx}    disy:${disy}    interval:${interval}"
    ./waf --run "scratch/test_constant --disy=${disy} --disx=${disx} --interval=${interval}" > /dev/null
    mv constant.log "scratch/data/constant/constant-${disx}-${disy}-${interval}.log"
done
done
done
