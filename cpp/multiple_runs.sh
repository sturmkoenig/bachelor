#!/bin/bash

for i in {80000..200000..10000}
do
    echo bin/xps_ex 5 $i
    bin/xps_ex 5 $i
done
