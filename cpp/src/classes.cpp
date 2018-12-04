#include "classes.h"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <experimental/random>
#include <fstream>
#include <iostream> 


Heart_Simulation::Heart_Simulation(double _dt, double _dx, double _D, int _Lx, int _Ly, int _time)
{
    dt = _dt;
    dx = _dx;
    D = _D;
    Lx = _Lx;
    Ly = _Ly;
    time = _time;
}

std::string Heart_Simulation::namemodel()
{
    std::string name = "HS";
    return name;
}

void Heart_Simulation::add_rec(int x, int y)
{
    all_rec.push_back( receiver(x,y) );
}

void Heart_Simulation::save_voltage(std::vector<int> pos)
{
    std::ofstream data;
    data.open("voltage.dat", std::ios::app);
    data << _time << "\t" <<  A[0](pos[0], pos[1]) << std::endl;
    data.close();
    return;
}

void Heart_Simulation::set_sensors(int x1, int y1, int x2, int y2, int tau)
{
    all_trans.push_back(transmitter(x1, y1));
    all_rec.push_back(receiver(x2, y2));

    link_trans LA= {&all_trans[0],tau};
    all_rec[0].set_transmitter(LA);
}

void Heart_Simulation::fast_pacing(int x, int y, int freq)
{
    all_rec.push_back(receiver(x, y, freq));
    return;
}

void Heart_Simulation::self_excited(int dist,int tau)
{

    all_trans.push_back(transmitter(150, 150+dist));
    all_rec.push_back(receiver(150,150));
    //all_rec[0].set_external_force( [](int time) {if(time<1){return 1.;}else{return 0.;}});
    //all_trans[1].name_of_file = std::to_string(dist) + "_" + std::to_string(tau) + ".dat";
    link_trans LA= {&all_trans[0],tau};
    all_rec[0].set_transmitter(LA);
    this->name_of_file = this->namemodel() + "_" + std::to_string(dist) + "_" + std::to_string(tau) + ".dat";
    return;
}

void Heart_Simulation::set_sensors(int number_of_pairs)
{
    if(number_of_pairs == 0)
        return;

    Gnuplot gp;
    plot plt(gp);

    int mb;
    double mx, my;
    arma::mat& v = A[0];
    plt.heatmap(v);

    for(int i=0; i<number_of_pairs; i++)
    {

        gp.getMouse(mx, my, mb, "Left Click for transmitter");
        std::cout << "mx  = " << mx << "my = " << my << "\n";
        int POS_TRANS[2] = { (int) my, (int) mx };

        gp.getMouse(mx, my, mb, "Left Click for receiver");
        std::cout << "mx  = " << mx << "my = " << my << "\n";
        int POS_REC[2] = {(int) my, (int) mx};

        all_rec.push_back(receiver(POS_REC[0], POS_REC[1]));
        all_trans.push_back(transmitter(POS_TRANS[0], POS_TRANS[1]));

    }

    for(unsigned int i=0; i<all_rec.size(); i++)
    {
        link_trans LK = { &all_trans[i], 0 };
        all_rec[i].set_transmitter(LK);
        all_trans[i].set_receiver(&all_rec[i]);
    }
    return;
}

void Heart_Simulation::set_sensors(int constelation, std::vector<int> tau)
{
    if(constelation==2)
    {
        all_trans.push_back(transmitter(30,30));	
        all_trans.push_back(transmitter(120,30));
        all_rec.push_back(receiver(30, 120));
        all_rec.push_back(receiver(120, 120));

        all_trans[0].set_receiver(&all_rec[0]);
        all_trans[1].set_receiver(&all_rec[1]);
        link_trans LA= {&all_trans[0], tau[0]};
        all_rec[0].set_transmitter(LA);
        link_trans LB= {&all_trans[1], tau[1]};
        all_rec[1].set_transmitter(LB);
    }

    if(constelation==3)
    {
        all_trans.push_back(transmitter(130,130));	
        all_rec.push_back(receiver(20, 130));
        all_rec.push_back(receiver(20, 20));
        all_rec.push_back(receiver(130, 20));
        for(int i=0; i<3; i++)
        {
            link_trans LK = {&all_trans[0], tau[i]};
            all_trans[0].set_receiver(&all_rec[i]);
            all_rec[i].set_transmitter(LK);
        }

    }
}

void Heart_Simulation::add_trans(int x, int y)
{
    all_trans.push_back( transmitter(x,y) );
}

bool Heart_Simulation::load()
{
    return false;
}

void Heart_Simulation::euler(std::vector< matrix >& B)
{
    return;
}

void Heart_Simulation::integrator()
{
    std::vector< matrix > B = A;		
    euler(B);
    B[0] += laplace();
    A = B;
    boundary_conditions();
}

void Heart_Simulation::boundary_conditions(void)
{
    for(matrix& B:A)
    {
        B.col(0)=B.col(1);
        B.row(0)=B.row(1);
        B.col(B.n_cols-1)=B.col(B.n_cols-2);
        B.row(B.n_rows-1)=B.row(B.n_rows-2);
    }
}

void Heart_Simulation::save_frame(arma::mat& plot, std::string name)
{
    std::ofstream outfile;
    outfile.open(name, std::ios::out);
    int i,j;
    for (i = 0; i < Lx; i+=2)
    {
        for (j = 0; j < Ly; j+=2)
        {
            outfile << plot(i,j) << " ";
        }
        outfile << std::endl;
    }

    outfile.close();
}

void Heart_Simulation::save_given_state(arma::mat& plot, std::string name)
{
    std::ofstream outfile;
    outfile.open(name, std::ios::out);
    outfile << plot;
    outfile.close();
}

void Heart_Simulation::check_if_terminated(int num_ps, int& counter)	
{
    if(num_ps != 0)
    {
        counter = 0;
        return;
    } 
    else
        counter++;
    if(counter == 2000)
    { 
        std::ofstream outfile;
        outfile.open("termination_time.txt", std::ios::app);

        // termination time has failur of the skipped time steps because it checks only every frame
        outfile << "Termination time: " << this->_time - 2000; 
        outfile.close();
        this->time=0; 
    }
    return; 
}

void Heart_Simulation::simulation(const std::string visualization, const bool spiral_waves, const bool save_final_state, const bool check_finished, int freq, const int skip_frames)
{
    Gnuplot gp;
    class plot plt(gp);
    phasemap phase;
    phase_singularity_map PS(Lx, Ly);
    arma::mat &v = A[0];	
    arma::mat &h = A[1];
    int num_ps = 0;
    int counter = 0;
    int i=0;

    for(i=0; i<time; i++)
    {
        _time = i;

        for(transmitter& trans:all_trans)
            trans.take(A[0],i, critical);

        integrator();

        if(spiral_waves)
        {
            //spiral wave initialisation wizardry
            if(i<1800)
                A[1](arma::span(0, Lx-70), arma::span( Ly/2-20, Ly/2)).fill(0.); 
        }

        if(counter != 0)
        {
            phase.make_phasemap(v, h);
            PS.line_integral(phase);
            num_ps = PS.count_singularitys();
            check_if_terminated(num_ps, counter);
        }

        if( (i%skip_frames)==0)
        {	

            if(check_finished)
            {
                phase.make_phasemap(v, h);
                PS.line_integral(phase);
                num_ps = PS.count_singularitys();

                check_if_terminated(num_ps, counter);
            }

            if(visualization == "plot")
            {		
                gp << "set title \"" << i << "\"\n";
                arma::mat plot = v;
                // plot(arma::span(all_trans[0].x()-1, all_trans[0].x()+1), arma::span(all_trans[0].y()-1, all_trans[0].y()+1)).fill(1);
                // plot(arma::span(all_rec[0].x()-1, all_rec[0].x()+1), arma::span(all_rec[0].y()-1, all_rec[0].y()+1)).fill(0.1);
                plt.heatmap(plot);
            }
            else if(visualization == "save frames")
            {
                arma::mat plot = v;
                save_frame(plot, std::string(tmp_dir+"frame"+std::to_string(i)+".dat"));
            }
            else if(visualization == "save ekg")
            {
                std::ofstream psdt;
                psdt.open("/scratch15/lauer/ps_bp_t2e5/"+this->name_of_file, std::ios::app);
                psdt << _time << "\t" <<  pseudo_ekg() << std::endl;
                psdt.close();
            }
            else if(visualization == "plot with PS")
            {
                gp << "set title \"" << i << "\"\n";
                arma::mat plot = v + PS.data;
                // plot(arma::span(all_trans[0].x()-1, all_trans[0].x()+1), arma::span(all_trans[0].y()-1, all_trans[0].y()+1)).fill(1);
                // plot(arma::span(all_rec[0].x()-1, all_rec[0].x()+1), arma::span(all_rec[0].y()-1, all_rec[0].y()+1)).fill(0.1);
                plt.heatmap(plot);
            }
        }
    }
    if(i !=time ) 
    {	 
        throw not_ended(); 
    }
    if(save_final_state)
    {
        arma::mat svng_v = v;
        arma::mat svng_h = h;
        v.save(tmp_dir+"end_state_v.dat", arma::arma_binary);
        h.save(tmp_dir+"end_state_h.dat", arma::arma_binary);
        all_trans[0].dump_to_file(tmp_dir);

	// maybe do some rework so that multiple transmitters can be saved?
	for(auto& value: all_trans)
		value.dump_to_file(tmp_dir);

    }
    if(visualization == "last frame")
    {
	arma::mat plot = v;
	save_frame(plot, std::string(tmp_dir+"fs_plottabel.dat") );
    }
    //for main so that termination success is known
    if(check_finished)
    //if(time == 0)
    //{
    //    std::ofstream outfile;
    //    outfile.open("termination_time.txt", std::ios::app);

    //    // termination time has failur of the skipped time steps because it checks only every frame
    //    outfile << " with freq " << freq <<  std::endl; 
    //    outfile.close();
    //}
    return;
}

matrix Heart_Simulation::laplace()
{
    matrix B;

    for(int i=1; i<Lx-1; i++)
    {
        for(int j=1; j<Ly-1; j++)
        {
            // seiten 
            B(i,j) = 4 * ( A[0](i+1,j) + A[0](i-1, j) + A[0](i, j+1) + A[0](i, j-1) )	
                // ecken
                + ( A[0](i+1, j+1) + A[0](i-1, j-1) + A[0](i+1, j-1) + A[0](i-1, j+1) )	
                // mitte
                - 20 * A[0](i,j);	
        }
    }

    B = dt*D/(6*dx*dx) * B;
    return B;
}
double Heart_Simulation::pseudo_ekg()
{
    double pseudo_freq = arma::accu(A[0])/(Lx*Ly); 
    return pseudo_freq;
}

