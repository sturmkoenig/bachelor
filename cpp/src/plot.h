#pragma	once

#include <vector>
#include <armadillo>

#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"
#include "param.h"

class plot
{
protected:
	Gnuplot& gp;
	std::vector< double > data;
	int Lx = MAT_SIZE_X;
	int Ly = MAT_SIZE_Y;

public:
	plot(Gnuplot& gnuplt): gp(gnuplt){ }
	void AP( std::vector< std::pair<double, double> >);
	void heatmap(arma::mat&);
	void v_vs_h( std::vector< std::pair<double, double> > );
	void phasemap(arma::mat&, double*);
};
