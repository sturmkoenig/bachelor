#pragma once

#include <armadillo>
#include "param.h"

struct perturbation
{
protected:
    // time the system should be disturbed
    int t;
    // position x
    int x;
    // position y
    int y;
    // duration of pulses
    int d;
    // frequenzy of pulses
    int f;
    // choose if points should be ranomised or not
    bool random=true;
public:
    // default constructor
    perturbation();
    // constructor for modelling every variable of this kind of perturbation
    perturbation(int,int,int,int,int);
    // the force function used by the integrator
    void force(matrix& a, int time);

};
