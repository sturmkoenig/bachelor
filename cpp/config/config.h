#include <string>

const std::string visualization  = "plot";

// if true load the approriate files out of the temporary folder
const bool load = false;

// variable that turns on or off spiral waves
const bool spiral_wave = false;

// make video right away? not available on the cluster
const bool make_video = false;

// save the last calculated step to tmp_dir in armadillo readable form so the simulation can be continued
const bool save_final_state = false;

// check if spiral waves are terminated 
const bool check_finished = false;

// skiped frames, important for every visualization and also set how often termination of SP is checked
const int skpfr = 30;

// perturbe the system?
const bool perturbed = false;

// initial form of the spiral wave
const std::string initial_form = "sp";
