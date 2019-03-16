#pragma once
#include <string>
#include <armadillo>
#include <utility>

//#include "sensor.h"

#define PI 3.141592

// size of matrix
// declared here so that matrizes can be allocated at compil time
const int MAT_SIZE_X = 300;
const int MAT_SIZE_Y = 300;

// for convience so you don't have to write out this ugly term 
typedef arma::mat::fixed<MAT_SIZE_X,MAT_SIZE_Y> matrix;
typedef std::pair< int, int > cordinate;

const std::string PATH_TO_PARAM = ("../parameters/");
const std::string PATH_TO_STATES = ("../parameters/");
