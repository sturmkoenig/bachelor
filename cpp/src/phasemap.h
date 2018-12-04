#pragma once
#include <armadillo>
#include "plot.h"
#include "gnuplot-iostream.h"
#include "param.h"

class phasemap
{
protected:
	arma::mat data;	
	double v_off;
	double h_off;
public:
	phasemap(double ov = 0.25, double oh = 0.4)
	{
		v_off = ov;
		h_off = oh;
	}

	void make_phasemap(arma::mat& v, arma::mat& h)
	{
		data = atan2(h-h_off,v-v_off);
	}

	void plt_phase(Gnuplot& gp)
	{
		plot plt(gp);
		double range[2] = {-PI/2, PI/2};
		plt.phasemap(data, range);
	}

	int x_dim(){ return data.n_rows;}

	int y_dim(){ return data.n_cols;}

	double val(int i, int j)
	{
		return data(i, j);
	}
};
