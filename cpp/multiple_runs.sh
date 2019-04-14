#!/usr/bin/zsh


for i in {1040..1082}
do
    bin/xps_ex 7 $i &
    bin/xps_ex 7 $(($i+1)) &
    bin/xps_ex 7 $(($i+2)) &
    bin/xps_ex 7 $(($i+3)) &
    bin/xps_ex 7 $(($i+4)) &
    bin/xps_ex 7 $(($i+5)) &
    bin/xps_ex 7 $(($i+6)) &
    wait
done

for i in {990..1044}
do
    bin/xps_ex 8 $i &
    bin/xps_ex 8 $(($i+1)) &
    bin/xps_ex 8 $(($i+2)) &
    bin/xps_ex 8 $(($i+3)) &
    bin/xps_ex 8 $(($i+4)) &
    bin/xps_ex 8 $(($i+5)) &
    bin/xps_ex 8 $(($i+6)) &
    wait
done

for i in {958..1005}
do
    bin/xps_ex 9 $i &
    bin/xps_ex 9 $(($i+1)) &
    bin/xps_ex 9 $(($i+2)) &
    bin/xps_ex 9 $(($i+3)) &
    bin/xps_ex 9 $(($i+4)) &
    bin/xps_ex 9 $(($i+5)) &
    bin/xps_ex 9 $(($i+6)) &
    wait
done

for i in {932..966}
do
    bin/xps_ex 10 $i &
    bin/xps_ex 10 $(($i+1)) &
    bin/xps_ex 10 $(($i+2)) &
    bin/xps_ex 10 $(($i+3)) &
    bin/xps_ex 10 $(($i+4)) &
    bin/xps_ex 10 $(($i+5)) &
    bin/xps_ex 10 $(($i+6)) &
    wait
done

for i in {890..928}
do
    bin/xps_ex 11 $i &
    bin/xps_ex 11 $(($i+1)) &
    bin/xps_ex 11 $(($i+2)) &
    bin/xps_ex 11 $(($i+3)) &
    bin/xps_ex 11 $(($i+4)) &
    bin/xps_ex 11 $(($i+5)) &
    bin/xps_ex 11 $(($i+6)) &
    wait
done
