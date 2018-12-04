#pragma once

#include <iostream>
#include <vector>
#include <armadillo>
#include <utility>

#include "phasemap.h"
#include "param.h"
#include "gnuplot-iostream.h"
#include "plot.h"

class phase_singularity_map
{
public:
	arma::mat data;
	std::vector<cordinate> iter; 
	phase_singularity_map(int Lx, int Ly)
	{ 
		data.zeros(Lx,Ly);
		//cordinate a1, b1, c1, d1, e1, e2, e3, e4, e5, d5, c5, b5, a5, a4, a3, a2;
		//a1 = std::make_pair(-2,-2);
		//b1 = std::make_pair(-2,-1);
		//c1 = std::make_pair(-2,0);
		//d1 = std::make_pair(-2,1);
		//e1 = std::make_pair(-2,2);
		//e2 = std::make_pair(-1,2);
		//e3 = std::make_pair(0,2);
		//e4 = std::make_pair(1,2);
		//e5 = std::make_pair(2,2);
		//d5 = std::make_pair(2,1);
		//c5 = std::make_pair(2,0);
		//b5 = std::make_pair(2,-1);
		//a5 = std::make_pair(2,-2);
		//a4 = std::make_pair(1,-2);
		//a3 = std::make_pair(0,-2);
		//a2 = std::make_pair(-1,-2);
		//

		//iter.push_back(a1);
		//iter.push_back(b1);
		//iter.push_back(c1);
		//iter.push_back(d1);
		//iter.push_back(e1);
		//iter.push_back(e2);
		//iter.push_back(e3);
		//iter.push_back(e4);
		//iter.push_back(e5);
		//iter.push_back(d5);
		//iter.push_back(c5);
		//iter.push_back(b5);
		//iter.push_back(a5);
		//iter.push_back(a4);
		//iter.push_back(a3);
		//iter.push_back(a2);
		// ! for smaller integration radie pression
		cordinate o_l, o_m, o_r, m_l, m_r, u_l, u_m, u_r;
		o_l = std::make_pair(-1, -1);
	   	o_m = std::make_pair(-1, 0);
	   	o_r = std::make_pair(-1, 1);
	   	m_l = std::make_pair(0, -1);
	   	m_r = std::make_pair(0, 1);
	   	u_l = std::make_pair(1, -1);
	   	u_m = std::make_pair(1, 0);
	   	u_r = std::make_pair(1, 1);
		iter.push_back(o_l);
		iter.push_back(o_m);
		iter.push_back(o_r);
		iter.push_back(m_r);
		iter.push_back(u_r);
		iter.push_back(u_m);
		iter.push_back(u_l);
		iter.push_back(m_l);
	}

	void print(Gnuplot& gp)
	{ 
		plot plt(gp);
		double range[2] = { -1, 1};
		plt.phasemap(data, range);
	}

	void line_integral(phasemap&);
	int count_singularitys();

};

