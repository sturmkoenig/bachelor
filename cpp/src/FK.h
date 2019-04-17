#pragma once

#include <armadillo>
#include <vector>
#include <string>

#include "param.h"
#include "classes.h"

class FK : public Heart_Simulation
{
protected:
	matrix v;
	matrix f;
	matrix s;

	double tau_fast;
	double tau_ung;		
	double tau_slow;	
	double tau_sopen;		
	double tau_sclose;		
	double tau_fopen;		
	double tau_fclose;		
	double V_sgate;
	double V_fgate;		
	double V_crit;
	double V_sig;	
	double V_out;		
	double kappa;
public:
	FK(double _dt, double _dx, double _D, int _Lx, int _Ly, int _time, double _tau_fast, double _tau_ung, double _tau_slow, double _tau_sopen, double _tau_sclose, double _tau_fopen, double _tau_fclose, double _V_sgate, double _V_fgate, double _V_crit, double _V_sig, double _V_out, double _kappa);
	void euler(std::vector< matrix >&) override; 

	bool load() override;
	bool load(std::string);
	std::string namemodel() override;
	matrix f_inf();
	matrix tau_f();
	matrix s_inf();
	matrix tau_s();
	matrix Q();
	matrix S();
	matrix J_fast();
	matrix J_slow();
	matrix P();
	matrix J_ung();
	matrix dv();
	matrix df();
	matrix ds();
};


