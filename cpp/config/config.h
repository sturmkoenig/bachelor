#include <string>
#include "../src/param.h"

#define STABLE

/*------------------------------------------------------------------------------------------------
*        Generell configurations 
*
*       visualization can be one of the follwing
*           -plot = plots via Gnuplot
*           -save frames = periodically saves voltage to tmp_dir/name_of_file
*           -save ekg = periodically  saves pseudo_ekg to txt file at ./scratch15/pseudo_ekg/
*           -plot with PS = plot with Phasesingularities
*           -save signal = saves the signal of EVERY pixel starting 10000 timesteps before the simulation ends
*	    -save specific signal = save the signal at the in classes.cpp specified place
*	    -last frame = plotable of last frame
*	    -output PS
*	    -output phasemap
*	    -output number PS
*       everything else yields no visualisation or save
 ------------------------------------------------------------------------------------------------*/
// pick one option from above
const std::string visualization  = "plot";

// sets the Temporary dire everything is written to
const std::string TEMP_DIR = "/scratch15/lauer/tmp/";

// if true load the approriate files out of the temporary folder
const bool load = false;

// if set to true not the standard path is taken but rahter file_load_path
const bool load_specific_file = false;

// the path that is loaded when load_specific_file is true
const std::string file_load_path="/home/sturmkoenig/Documents/Physik/Bachlor/saved_states/MS_chaotic_dynamic_2e4ms/";
// const std::string file_load_path="/home/sturmkoenig/Documents/Physik/Bachlor/saved_states/MS_chaotic_dynamic_2e4ms/pulse_for_4e5ms_with_5_1143/";
// const std::string file_load_path="/home/sturmkoenig/Documents/Physik/Bachlor/plots/new_stability_test/after_perturbation/tmp/stability_test300x300_MS_unstable_self_excited_d_7_tau_3050/";

// make video right away? not available on the cluster
const bool make_video = false;

// save the last calculated step to tmp_dir in armadillo readable form so the simulation can be continued
const bool save_final_state = false;

// check if spiral waves are terminated 
const bool check_finished = false; 

// skiped frames, important for every visualization and also set how often termination of SP is checked
const int skpfr = 50;

// perturbe the system?
const bool perturbed = false;

// sets a trigger in classes.cpp for flushing out a phase at a certain point
const bool action_potential = false;

// sets the time a system is simulated
const int SIMULATION_TIME = 12500000;

// initial form of the spiral wave. Options are "velocity_measurement" "sp plane" and "sp" "pulse"
// REMEMBER IF LOAD=TRUE NO INITIAL_FORM!!!
const std::string initial_form = "sp plane";

const std::string NAME_ADDON = "find_working_param" + std::to_string(MAT_SIZE_X) + "x" + std::to_string(MAT_SIZE_Y);
