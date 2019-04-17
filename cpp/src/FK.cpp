#include "FK.h"


FK::FK(double _dt, double _dx, double _D, int _Lx, int _Ly, int _time, double _tau_fast, double _tau_ung, double _tau_slow, double _tau_sopen, double _tau_sclose, double _tau_fopen, double _tau_fclose, double _V_sgate, double _V_fgate, double _V_crit, double _V_sig, double _V_out, double _kappa ) : Heart_Simulation(_dt, _dx, _D, _Lx , _Ly, _time)
{
	using namespace arma;
	v.fill(0);
	f.fill(1);
	s.fill(1);
	v(span(0, int(Lx/2)), span(0,int(Ly/2)-1)).fill(.5);
	A.push_back(v);
	A.push_back(f);
	A.push_back(s);

	tau_fast=_tau_fast;
	tau_ung=_tau_ung;		
	tau_slow=_tau_slow;	
	tau_sopen=_tau_sopen;		
	tau_sclose=_tau_sclose;		
	tau_fopen=_tau_fopen;		
	tau_fclose=_tau_fclose;		
	V_sgate=_V_sgate;
	V_fgate=_V_fgate;		
	V_crit=_V_crit;
	V_sig=_V_sig;	
	V_out=_V_out;		
	kappa=_kappa;
}

std::string FK::namemodel()
{
    std::string name = "FK";
    return name;
}

bool FK::load(std::string load_path)
{
    return false;
}

bool FK::load()
{
    bool a,b,c;
    a = A[0].load(tmp_dir+"end_state_v_fk.dat");
    b = A[1].load(tmp_dir+"end_state_f_fk.dat");
    c = A[2].load(tmp_dir+"end_state_s_fk.dat");

    for( auto& vec:all_trans )
	vec.read_from_file(tmp_dir);

    if(a && b && c)
	return true;
    else
	return false;
}


matrix FK::f_inf()
{
	arma::mat A = arma::conv_to< arma::mat >::from(V_fgate > v);
	return A;
}

matrix FK::tau_f()
{
	arma::mat A = arma::conv_to< arma::mat >::from(v > V_fgate);
	A *= (tau_fclose - tau_fopen);
	matrix B;
	B.fill(tau_fopen);
	A += B; 
	return A;
}

matrix FK::s_inf()
{
	arma::mat A = arma::conv_to< arma::mat >::from(V_sgate > v);
	return A;
}

matrix FK::tau_s()
{
	arma::mat A = arma::conv_to< arma::mat >::from(v > V_sgate);
	A *= (tau_sclose - tau_sopen);
	matrix B;
	B.fill(tau_sopen);
	A += B; 
	return A;
}

matrix FK::P()
{
	arma::mat A = arma::conv_to< arma::mat >::from(v > V_sgate);
	A %= (1 - v/V_out);
	matrix B = v/V_out;
	A += B; 
	return A;
}

matrix FK::Q()
{
		
	arma::mat A = arma::conv_to< arma::mat >::from(v > V_sgate);
	A %= (v-V_crit) % (1-v);
	return A;
}

matrix FK::S()
{
	matrix A;
	A.ones();
	A += tanh(kappa * (v -V_sig))/2;	
	return A;
}

matrix FK::J_fast()
{
	matrix A;
	A = -f % Q() / tau_fast;
	return A;
}

matrix FK::J_slow()
{
	matrix A;
	A = s % S()  / tau_slow;
	return A; 
}

matrix FK::J_ung()
{
	matrix A = P() / tau_ung;
	return A; 
}

matrix FK::dv()
{
	matrix A = -(J_fast()  + J_slow()  + J_ung() );
	return A; 
}

matrix FK::df()
{
	matrix A = (f_inf() - f) / tau_f() ;
	return A; 
}

matrix FK::ds()
{
	matrix A = (s_inf() - s)/tau_s() ;
	return A;
}

// in A the 3 fields are stored in order v, f, s, where B is justed a helping variable
void FK::euler(std::vector< matrix >& B)
{
	v = A[0];
	f = A[1];
	s = A[2];

	B[0] = A[0] + dt * dv();
	B[1] = A[1] + dt * df();
	B[2] = A[2] + dt * ds();

	if(_time > -1)
	{
	    for(receiver rec:all_rec)
		rec.force(A[0], _time, dt);
	    prtbtin.force(A[0], _time);
	}

	return;
}

