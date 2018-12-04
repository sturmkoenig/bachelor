#include <armadillo>
#include <vector>
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

#define SIMTYP FK

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

        // PREFIX is the dir name of output video if make_video is set to true
        std::string PREFIX;

        // Name of the output file in dependency of simulation type and commandline parameters
        std::string name_of_file; 
        
        // the temporary dire where files are saved (usally scratch) 
        std::string tmp_dir;

	bool spiral_wave = false;

	if(initial_form == "sp")
	    spiral_wave = true;

	
         
       // initialize mitchell schaeffer system. Parameters can be looked at in MS.h
       //SIMTYP simulation( 0.2,    0.1, 0.001,MAT_SIZE, MAT_SIZE, 2000000,    0.13,   .4,    10.,         130.,       150.,       0.0, initial_form);
	/*	
	 Fenton-karma Model for latter use  
	 */
	//				dt,			dx,				D,	
	SIMTYP simulation = FK { 0.2,		 	0.1, 			1e-3, 
	//				Lx,				Ly,			time, 
					MAT_SIZE, 	MAT_SIZE, 		200000,
	// 			tau_fast,		tau_ung, 		tau_slow,		
					.25,			130,			127,
	// 			tau_sopen,		tau_sclose,		tau_fopen,		
					80,				1000,			18,
	// 			tau_fclose,		V_sgate,		V_fgate,		
					10,				.13,			.13,		
	// 			V_crit,			V_sig,			V_out,				
					.13,			.85,			.1,			
	//			kappa
					10};

       /*------------------------------------------------------------------------------------------------
          *         diffrent kinds of simulations can be addresed by diffrent argument counts
          *         the diffrent kinds till now are listed below
          *         for every type diffrent temporary dires and filenames are set
         ------------------------------------------------------------------------------------------------*/

        // self-exiced periodicity test
        if(argc == 3)
        {
            dist = atoi(argv[1]);
            tau = atoi(argv[2]);
            PREFIX = "a";
            name_of_file = simulation.namemodel() + "_bp_t_" + std::to_string(dist) + "_" + std::to_string(tau) + "ms";
            tmp_dir = "/scratch15/lauer/tmp/" + name_of_file + "/";
            simulation.tmp_dir = tmp_dir;
            simulation.self_excited(dist,tau);
            std::cout << "starting simulation with " << dist << "\t" << tau << "\t\n";
            std::cout << "temporary dire is " << tmp_dir << std::endl;
        }

        // fast pulsing test
        else if(argc == 4)
        {
            x1 = atoi(argv[1]); 
            y1 = atoi(argv[2]); 
            freq = atoi(argv[3]);
            PREFIX = "a";
            name_of_file = simulation.namemodel() + "_bp_fp_"+std::to_string(x1) + "," + std::to_string(y1) + "_"  + std::to_string(freq) + "ms";
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
        
        // making sure the folder exsist when generating video
        if(visualization == "save frames" || visualization == "last frame" || save_final_state == true)	
            int del_1 = std::system(("mkdir " + tmp_dir).c_str());

        // tries to load the last calculated state
        if(load)
        {
	    if(!simulation.load())
	    {
		std::cout << "data could not be loaded\n";
		return 0;
	    }
            std::cout << "data loaded succesfully\n";
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
            simulation.simulation(visualization, spiral_wave, save_final_state, check_finished, freq, skpfr);
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
