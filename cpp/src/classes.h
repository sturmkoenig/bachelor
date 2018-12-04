#pragma once
#include <vector>
#include <armadillo>
#include <string>
#include <exception>

#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"
#include "sensor.h"
#include "param.h"
#include "perturbation.h"
#include "phasemap.h"
#include "phase_singularity.h"
#include "plot.h"

struct not_ended: public std::exception
{
    const char* what() const throw () 
    {
        return "stopped simulation prematurely";
    }
};


class Heart_Simulation
{
protected:
	// here all parameters are stored but every class 
	// can get extra matrix pointers for convinency
	// note that A[0] has to be voltage
	std::vector< matrix > A;
        // time stepping
	double dt;
        // spatial stepping
	double dx;
        // diffusion constant
	double D;
	double critical = 0;
        // incase of storaging data name of output file
        std::string name_of_file;
        // Lx and Ly are MAT_SIZE by default
	int Lx;
	int Ly; 
        // the time the system should be simulated for
	int time;
        // here all sensors for the system can be stored
public:
	std::vector< receiver > all_rec;
	std::vector< transmitter> all_trans;
        perturbation prtbtin;
        std::string tmp_dir;
	void save_voltage(std::vector<int>);
        double pseudo_ekg();
        void fast_pacing(int, int, int);
	void save_given_state(arma::mat&, std::string);
	void save_frame(arma::mat&, std::string);
        void self_excited(int,int);
	int _time;
        void check_if_terminated(int, int&);
	void set_sensors(int);
	void set_sensors(int, std::vector<int>);
	void set_sensors(int, int, int, int, int);
	void boundary_conditions();
	virtual bool load();
	virtual std::string namemodel();
	virtual void euler(std::vector<matrix>&);
	Heart_Simulation(double,double,double,int,int,int);
	virtual void integrator();
	matrix laplace();
	virtual void simulation(const std::string,bool,bool,const bool,int,const int);
	void add_rec(int,int);
	void add_trans(int,int);
};	

