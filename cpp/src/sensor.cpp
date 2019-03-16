#include "sensor.h"
#include <algorithm>
#include <cmath>

#include <iostream>
#include <iterator>
#include <fstream>

using namespace arma;

/*--------------------------------------------------------------------------*/
//						Sensor
/*--------------------------------------------------------------------------*/
sensor::sensor(int x, int y)
{
    pos[0] = x;
    pos[1] = y;
    dim[0] = 1;
    dim[1] = 1;
    exakt_pos[0]=pos[0];
    exakt_pos[1]=pos[1];
    velocity = .001;
    final_pos[0] = 121;
    final_pos[1] = 150;
}

int sensor::x()
{
    return pos[0];
}

int sensor::y()
{
    return pos[1];
}

/*--------------------------------------------------------------------------*/
//				Receiver
/*--------------------------------------------------------------------------*/
receiver::receiver(int x, int y) : sensor(x, y)
{
    amplifier = 1;	
    external_force = [](int t) {return 0.*t;};
}

receiver::receiver(int x, int y, int freq) : sensor(x, y)
{
    amplifier = 1;	
    external_force = [freq](int t) 
    {
        double trie = std::sin(2*3.141592*t / freq);
        if(trie>0.80){
            return 1;
        } 
        else{ 
            return 0;
        }
    };
}

receiver::receiver(int x, int y, int freq, int t0, int t1) : sensor(x, y)
{
    amplifier = 1;	
    external_force = [freq,t0,t1](int t) 
    {
        double trie = std::sin(2*3.141592*t / freq);
        if(t>t0 && t<t1){
            if(trie>0.80){
                return 1;
            } 
            else{ 
                return 0;
            }
        }
        else{
            return 0;
        }
    };
}

void receiver::force(arma::mat& A, int time, double dt)
{
    double force = 0;

    for(link_trans& T:all_trans)
        force = amplifier * dt * (*T.trans).voltage_at_tau(T.tau);

    force += external_force(time);

    for(int i = -dim[0]; i <= dim[0] ; i++)
    {
        for(int j = -dim[1]; j <= dim[1]; j++)
        {
            A(pos[0]+i, pos[1]+j) += force;
        }
    }

    return;
}

void receiver::set_external_force(std::function<double (int)> f)
{
    external_force = f;
}

void receiver::set_transmitter(link_trans trans)
{
    all_trans.push_back(trans);
}

int receiver::delay_time()
{
    return (all_trans[0].tau);
}

/*--------------------------------------------------------------------------*/
//				Transmiter
/*--------------------------------------------------------------------------*/

int transmitter::time()
{
    return t2-t1;
}

transmitter::transmitter(int x, int y, int _ID) : sensor(x, y)
{
    state = 0;
    ID=_ID;
    tau = 0;
    t1 = 0;
    t2 = 0;
    std::fill(SAVE.begin(), SAVE.end(), 0);
    write_data = [](int time, double freq, int AP_duration, std::string filename){return;};
}

transmitter::transmitter(int x, int y, std::string _tmp_dir, int _ID) : sensor(x, y)
{
    ID=_ID;
    state = 0;
    tau = 0;
    t1 = 0;
    t2 = 0;
    std::fill(SAVE.begin(), SAVE.end(), 0);
    tmp_dir = _tmp_dir;
    write_data = [](int time, double voltage, int AP_duration,std::string target_file)
    {
        std::ofstream data;
        data.open(target_file, ios::app);
        data << time << "\t" << voltage << std::endl;
	data.close();
        return;
    };
}

void transmitter::dump_to_file(std::string path)
{
	std::ofstream outfile(path + "save_trans.txt", std::ios::out | std::ofstream::binary);
	std::ostream_iterator<double> osi{outfile," "};
	std::copy(SAVE.begin(), SAVE.end(), osi);
	outfile.close();
}

void transmitter::read_from_file(std::string path)
{
        std::vector<double> newVector;
	std::ifstream infile(path+"save_trans.txt", std::ios::in | std::ifstream::binary);
	std::istream_iterator<double> iter{infile};
	std::istream_iterator<double> end{};
	std::copy(iter, end, std::back_inserter(newVector));
	infile.close();

        SAVE = newVector;
        std::cout << "loaded saved data from :" << path << " into vector\n";
	return;
}

double transmitter::dist_TR()
{
    //double distance = std::sqrt( pow( x() - rec->x() , 2) + pow( y() - rec->y() , 2));
    return 1;
}



void transmitter::set_receiver(receiver* target_rec)
{
    all_rec.push_back(target_rec);
}


int transmitter::s() { 
    return state;
}


// save sensors current state at t and shifts previous states to t-1
int transmitter::take(arma::mat& A, int time, double critical_value)
{
    std::rotate(SAVE.rbegin(), SAVE.rbegin() + 1, SAVE.rend());
    double rec= mean( mean( A(span(pos[0]-1,pos[0]+1), span(pos[1]-1,pos[1]+1)) ) ) ;
    SAVE[0] = rec;
    if(time>1000000 && time%20 == 0)
        write_data(time, rec, 1,  "irrelevant");
    msr_frq(rec, time, critical_value);
    return 0;
}

int transmitter::t() {
    return t1;
}
int transmitter::last_excited(int time)
{
    int lst_exctd = time - t2;
    return lst_exctd;
}

void transmitter::write_AP(int time, double voltage, std::string fname)
{
    std::ofstream data;
    data.open(fname, ios::app);
    data << time << "\t" << voltage << std::endl;
    data.close();
}

void transmitter::msr_frq(double voltage, int time, double crit_val)
{
    if( state == 0 && voltage > crit_val)
    {
        t2 = t1;
        t1 = time; 
        state = 1;
        freq = t1-t2;
        write_data(time,freq,AP_duration, "sensor"+std::to_string(ID));
        return;
    }	

    else if( state == 0 && time%10000 && last_excited(time) > 10000)
    {
        throw DynamicTerminated();
    }
        
    else if(state == 1 && voltage < crit_val)
    {
        t2 = time;
        AP_duration=t2-t1;
        state = 0;
    }
    return;
}


double transmitter::voltage_at_tau(int tau)
{
    return SAVE[tau];
}

int transmitter::msr_time(double voltage, int time, double critical_value)
{
    if( state == 0 && voltage > critical_value)
    {
        t1 = time; 
        state = 1;
        return 1;
    }	

    else if(state == 1 && voltage < critical_value)
    {
        state = 0;
        t2 = time;
    }
    return 0;
}
