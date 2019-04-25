#pragma once
#include <vector>
#include <armadillo>
#include <string>
#include <exception>
#include <fstream>

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
	arma::mat::fixed<int(MAT_SIZE_X/2), int(MAT_SIZE_Y/2)> v_half;
        // time stepping
	double dt;
        // spatial stepping
	double dx;
        // diffusion constant
	double D;
	double critical = 0;
        // increase of storaging data name of output file
        std::string name_of_file;
        // Lx and Ly are MAT_SIZE by default
	int Lx;
	int Ly; 
        // the time the system should be simulated for
	int time;
        // here all sensors for the system can be stored
	void calc_v_half();
public:
	const int HALF_SIZE_X = int(MAT_SIZE_X/2);
	const int HALF_SIZE_Y = int(MAT_SIZE_Y/2);
	void save_signal(std::ofstream*,int x, int y);
	void save_signal(std::ofstream*);
	std::vector< receiver > all_rec;
	std::vector< transmitter> all_trans;
        perturbation prtbtin;
        std::string tmp_dir;
	void save_voltage(std::vector<int>);
        double pseudo_ekg();
	void velocity();
        void fast_pacing(int, int, int);
	void save_given_state(arma::mat&, std::string);
	void save_frame(arma::mat&, std::string);
        void self_excited(int,int vec_length=-1);
	int _time;
        void check_if_terminated(int, int&);
	void set_sensors(int);
	void set_sensors(int, std::vector<int>);
	void set_sensors(int, int, int, int, int);

	// function to set arbitrary combinations of transmitters and receivers 
	// negative numbers in links means not connected
	template <int num_rec, int num_trans> 
	void set_sensors(std::array<std::array<int, 2>, num_rec > pos_rec, std::array<std::array<int, 2>, num_trans > pos_trans,  arma::Mat<int>::fixed<num_rec,num_trans> links)
	{
	    for(int i=0; i<num_rec; i++)
	    {
		all_rec.push_back(receiver(pos_rec[i][0], pos_rec[i][1]));
	    }
	    for(int i=0; i<num_trans; i++)
	    {
		all_trans.push_back(transmitter(pos_trans[i][0], pos_trans[i][1]));
	    }
	    for(int i=0; i<num_rec; i++)
	    {
		for(int j=0; j<num_trans; j++)
		{
		    if( links(i,j) >= 0)
		    {
			link_trans Link = {&all_trans[j], links(i,j)};
			all_trans[j].set_receiver(&all_rec[i]);
			all_rec[i].set_transmitter(Link);
		    }
		}
	    }
	    return;

	};

	void boundary_conditions();
	virtual bool load(std::string path);
	virtual bool load();
	virtual std::string namemodel();
	virtual void euler(std::vector<matrix>&);
	Heart_Simulation(double,double,double,int,int,int);
	virtual void integrator();
	matrix laplace();
	virtual void simulation(const std::string,bool,bool,const bool,int,const int, const bool, int);
	void add_rec(int,int);
	void add_trans(int,int);
};	

