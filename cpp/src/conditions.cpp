#include "conditions.h"

// the no-flux boundary condition
void boundary_conditions(arma::mat& A)
{
    A.col(0)=A.col(1);
    A.row(0)=A.row(1);
    A.col(A.n_cols-1)=A.col(A.n_cols-2);
    A.row(A.n_rows-1)=A.row(A.n_rows-2);
    return;
}

// initial conditions
void initial_conditions(arma::mat& v, arma::mat& h)
{
    using namespace arma;
    int L = v.n_rows;
    v(span(0, int(L/2)), span(0,L-1)).fill(.2);
    h(span(0, L-1), span(0, int(L/2))).fill(.8);
    return;
}
