#include "MS.h"
#include <iostream>

MS::MS(double _dt, double _dx, double _D, int _Lx, int _Ly, int _time, double _v_gate, double _tau_in, double _tau_out, double _tau_open, double _tau_closed, double _a, const std::string initial_setup) : Heart_Simulation(_dt, _dx, _D, _Lx , _Ly, _time)
{
	using namespace arma;
	matrix v;
	matrix h;
        v.fill(_a);
        h.fill(1);

        if(initial_setup == "sp")
        {
	    // v(span(0, int(Lx/2)), span(0,Ly-1)).fill(.2);
            // h(span(0, Lx-1), span(0, int(Ly/2))).fill(.8);
	    v(span(0, int(Lx/2)+50), span(int(Ly/2)-50,int(Ly/2)-48)).fill(1.);
	    h(span(0, int(Lx/2)+50), span(int(Ly/2)-48,int(Ly/2)+50)).fill(.0);
        }

        else if(initial_setup == "pulse")
            v(span(149,151), span(149,151)).fill(1);
	
	A.push_back(v);
	A.push_back(h);
	v_gate = _v_gate;
	tau_in = _tau_in;
	tau_out = _tau_out;
	tau_open = _tau_open;
	tau_closed = _tau_closed;
	a = _a;
	critical = v_gate;
}

std::string MS::namemodel()
{
    std::string name = "MS";
    return name;
}

bool MS::load()
{
    bool a,b;
    a = A[0].load(tmp_dir+"end_state_v_fk.dat");
    b = A[1].load(tmp_dir+"end_state_f_fk.dat");

    for( auto& vec:all_trans )
	vec.read_from_file(tmp_dir);

    if(a && b)
	return true;
    else
	return false;
}
void MS::euler(std::vector< matrix >& A)
{
	matrix& v = A[0];
	matrix& h = A[1];
	arma::mat H = arma::conv_to< arma::mat >::from((v_gate-v)>0);
	A[0] = ( v + dt * ((h % ((v-a) % (v-a)) % (1-v))/ tau_in - (v-a)/tau_out) ); 
	A[1] = ( h + dt * ((H-h)/(tau_open - H * (tau_open - tau_closed))) ); 

        if(_time > -1)
        {
	    for(receiver& rec:all_rec)
		rec.force(A[0], _time, dt);
            prtbtin.force(A[0], _time);
        }
	return;
}
