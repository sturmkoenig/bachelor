#include <armadillo>
#include <vector>
#include <array>
#include <map>
#include <cmath>
#include <cstdio>
#include <string>
#include <utility>
#include <cstdlib>

#include "../config/config.h"
#include "classes.h"
#include "MS.h"
#include "FK.h"
#include "conditions.h"
#include "param.h"
#include "sensor.h"
#include "perturbation.h"

#define SIMTYP MS

int main(int argc, char *argv[])
{
        // here the various diffrent parameters maybe used in the simulation are declared for later use
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        int tau = 0;
        int freq = 0;
        int dist = 0;
	int save_steps = 0;

        // PREFIX is the dir name of output video if make_video is set to true
        std::string PREFIX;

        // Name of the output file in dependency of simulation type and commandline parameters
        std::string name_of_file; 
        
        // the temporary dire where files are saved (usally scratch) 
        std::string tmp_dir;

	bool spiral_wave = false;

	if(initial_form == "sp")
	    spiral_wave = true;

	if( (load || load_specific_file) && initial_form!="vanilla")
	{
	    std::cout << "ERROR: There is a initial_form even though data is beeing loaded\n";
	    std::cout << "Initial_form is: " << initial_form << std::endl;
	}
	
         
       // initialize mitchell schaeffer system. Parameters can be looked at in MS.h
       

#if defined UNSTABLE
	/*	    instable parameters		*/
       SIMTYP simulation( 0.2,    0.1, 0.001,MAT_SIZE_X, MAT_SIZE_Y, SIMULATION_TIME,    0.13,   .4,    10.,         130.,       150.,       0.0, initial_form);

#endif

       /*------------------------------------------------------------------------------------------------*/

#if defined STABLE
       /*	    stable parameters		*/
       SIMTYP simulation( 0.2,    0.1, 0.001,MAT_SIZE_X, MAT_SIZE_Y, SIMULATION_TIME,    0.13,   .3,    6.,         120.,       150.,       0.0, initial_form);
#endif


       /*------------------------------------------------------------------------------------------------
          *         diffrent kinds of simulations can be addresed by diffrent argument counts
          *         the diffrent kinds till now are listed below
          *         for every type diffrent temporary dires and filenames are set
         ------------------------------------------------------------------------------------------------*/

	if(argc == 1)
	{
	    std::array<std::array<int, 2>, 2> receivers = { { {150,10}, {150, 15} } };
	    std::array<std::array<int, 2>, 1> transmitters = { { {150,20} } };
	    arma::Mat<int>::fixed<2, 1> links = {-1, -1};
	    simulation.set_sensors<2, 1>(receivers, transmitters, links);
	    simulation.tmp_dir = "/scratch15/lauer/tmp/";
	}
	// wave velocity
	else if(argc == 2)
	{
	    std::cout << "measure wave propagation velocity\n";
	    name_of_file = simulation.namemodel() + "_velocity_measurment";
	    tmp_dir = "/scratch15/lauer/tmp/" + name_of_file + "/";
	    std::cout << "temporary dire is " << tmp_dir << std::endl;
	    simulation.velocity();
	}

       // self-except periodicity test
       else if(argc == 3)
        {
            dist = atoi(argv[1]);
            tau = atoi(argv[2]);
	    save_steps = 50000;
            PREFIX = "a";
            name_of_file = simulation.namemodel() + "_unstable_self_excited_d_" + std::to_string(dist) + "_tau_" + std::to_string(tau); 
            tmp_dir = "/scratch15/lauer/tmp/" + name_of_file + "/";
            simulation.tmp_dir = tmp_dir;
            simulation.self_excited(dist,tau);
            std::cout << "starting simulation with " << dist << "\t" << tau << "\n";
            std::cout << "temporary dire is " << tmp_dir << std::endl;
        }

        // fast pulsing test
        else if(argc == 4)
        {
            x1 = atoi(argv[1]); 
            y1 = atoi(argv[2]); 
            freq = atoi(argv[3]);
            PREFIX = "a";
            name_of_file = simulation.namemodel() + "_bp_ACP_"+std::to_string(x1) + "," + std::to_string(y1) + "_"  + std::to_string(freq) + "ms";
            tmp_dir = "/scratch15/lauer/tmp/" + name_of_file + "/";
            simulation.tmp_dir = tmp_dir;
            simulation.fast_pacing(x1, y1, freq);
        }

        // simulation with spiral waves and sensors at points x and y 
        else if(argc == 6)
        {
            x1 = atoi(argv[1]); 
            y1 = atoi(argv[2]); 
            x2 = atoi(argv[3]); 
            y2 = atoi(argv[4]); 
            tau = atoi(argv[5]); 
            PREFIX = "a";
            name_of_file = simulation.namemodel() + "_bp_2s_"+ std::to_string(x1) + "," + std::to_string(y1) +  "_" + std::to_string(x2) + "," + std::to_string(y2) + "_" + std::to_string(tau) + "ms";
            tmp_dir = "/scratch15/lauer/tmp/" + name_of_file + "/";
            simulation.tmp_dir = tmp_dir;
            simulation.set_sensors(x1, y1, x2, y2, tau);
        }

        // in case not matching
        else
        {
            std::cout << "wrong option count" << std::endl;
            return 0;
        }

	// make the folder -p is for making parrent directorys 
	std::cout << "mkdir " << tmp_dir << std::endl;
	std::system(("mkdir -p " + tmp_dir).c_str());

	// tries to load the last calculated state
	if(load && !load_specific_file)
	{
	    if(!simulation.load())
	    {
		std::cout << "data could not be loaded\n";
		return 0;
	    }
	    if(save_final_state == true)
		std::system(("rm " + tmp_dir + "end_state*").c_str());
	    std::cout << "data loaded succesfully\n";
	}

	else if(load_specific_file)
	{
	    if(!simulation.load(file_load_path))
	    {
		std::cout << "the specified file could not be loaded\n" << file_load_path << std::endl;
	    }
	}
	
	if(visualization == "save signal")
	{
	    // beforhand delete previous folder
	    std::system(("rm -r " + simulation.tmp_dir +"Signal/").c_str());
	   // make new one
	    std::system(("mkdir -p " + simulation.tmp_dir +"Signal/").c_str());
	}
            
         /*------------------------------------------------------------------------------------------------
                                        Perturbation class
         ------------------------------------------------------------------------------------------------*/
        if(perturbed)
        {
            try
            {
                // perturbation is initialised with time,x,y,duration of pulse, frequenzy of pulses)
                perturbation pertbtn(1000,10,10,300,100000);
                simulation.prtbtin = pertbtn;

            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return 0;
            }
        }

            
         /*------------------------------------------------------------------------------------------------
          *         Here Here the Simulation is carried out
          *         various diffrent failures can be catched
          *         some are in sensors others are more specific
         ------------------------------------------------------------------------------------------------*/

        try
        {
            simulation.simulation(visualization, spiral_wave, save_final_state, check_finished, freq, skpfr, action_potential, save_steps);
        }

        catch(const std::exception& e)
        {
            std::cout << "Terminated\n"<< e.what() << std::endl;
            return 0;
        }

        
        // not working on cluster | make_video is set at the configuring section
	if(make_video)
        {
            std::string mk_dir = "mkdir " + tmp_dir + " /scratch15/lauer/video/"+PREFIX;
            int del_2 = std::system((mk_dir).c_str());
	    std::string command =   "python3 plt.py " + PREFIX + "/" +  name_of_file + " " + tmp_dir;
	    std::cout << command << std::endl;
	    int del_3 = std::system((command).c_str());
        }		
        
	return 1;
}
