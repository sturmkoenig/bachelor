#include <armadillo>
#include <string>

#include "param.h"
#include "perturbation.h"
#include "classes.h"

class MS : public Heart_Simulation
{
protected:
	double v_gate;
	double tau_in;
	double tau_out;
	double tau_open;
	double tau_closed;
	double a;
public:
	void euler(std::vector <matrix>&) override;
	std::string namemodel();
	MS(double _dt, double _dx, double _D, int _Lx, int _Ly, int _time, double _v_gate, double _tau_in, double _tau_out, double _tau_open, double _tau_closed, double _a, const std::string initial_form);
	bool load();
	std::vector< matrix > force_MS(void);
	void initial_conditions();

};
