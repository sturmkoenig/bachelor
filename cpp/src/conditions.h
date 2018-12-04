#pragma once

#include<armadillo>
#include "param.h"

void boundary_conditions(arma::mat& A);
void initial_conditions(arma::mat& v, arma::mat& h);
