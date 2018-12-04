#!/bin/bash

# check if connected per wire
network=$(ifconfig | grep -Fq "enx000ec6ce2244")

if [ $network=0 ]; then
    sshfs lauer@epona01:/home/lauer/smaxxhome/Documents/Heart_Simulation/cpp/src ./src
    sshfs lauer@epona01:/scratch15/ /scratch15
else
    sshfs lauer@login.lfpn.ds.mpg.de:/home/lauer/smaxxhome/Documents/Heart_Simulation/cpp/src ./src
    sshfs lauer@login.lfpn.ds.mpg.de:/scratch15/ /scratch15
fi
