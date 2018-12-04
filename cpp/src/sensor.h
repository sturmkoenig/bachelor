#pragma once

#include <armadillo>
#include <vector>
#include <string>
#include <exception>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

#include "param.h"

class transmitter;
class receiver;

struct link_trans
{
    transmitter* trans;
    int tau;
};

struct DynamicTerminated : public std::exception
{
    const char* what() const throw () 
    {
        return "10000 or more steps since excitation";
    }
};

class sensor
{

    protected:
        std::vector<int> pos{std::vector<int>(2, 0)};   //position of sensor
        std::vector<int> dim{std::vector<int>(2, 0)};   //length of rectangular sensor sides
        std::vector<double> exakt_pos{std::vector<double>(2, 0.)};
        std::vector<int> final_pos{std::vector<int>(2, 0)};
        double velocity;
        int tau;
        int t1; 
        int t2; 

    public:
        sensor(int x, int y);
        int x();
        int y();
        std::string tmp_dir;
        void move(std::vector<int> new_pos);
        void move_sensor(void);
};




class transmitter : public sensor
{
    protected:
        int state;		//if 0: can measure dt, if 1: wait till v<v_crit
        std::vector<double> SAVE{std::vector<double>(4001)};
        std::vector< receiver* > all_rec;
        std::function<void (int,double,int,std::string)> write_data;
    public:
        void write_AP(int,double, std::string fname);
        int last_excited(int time);
        void dump_to_file(std::string);
        void read_from_file(std::string);
        int time();
        int freq;
        int AP_duration;
        transmitter(int, int);
        transmitter(int, int,std::string);
        int take(arma::mat& A, int time, double critical_value);
        int msr_time(double voltage, int time, double critical_value);
        void msr_frq(double,int,double);
        double dist_TR();
        double voltage_at_tau(int tau);
        void load_SAVE();
        int t();
        int s();
        int tau;
        void set_receiver(receiver* target_rec);
	std::string name_of_file;
};



class receiver : public sensor
{
    protected:
        std::function<double (int)> external_force;
        double amplifier;
        std::vector<link_trans> all_trans;	
    public:
        receiver(int x, int y);
        receiver(int x, int y, int freq);
        receiver(int x, int y, int freq, int t0, int t1);
        void force(arma::mat& A, int t, double dt);
        void set_transmitter(link_trans);
        void set_external_force(std::function<double (int)>);
        int delay_time();
};

class transceiver : public receiver , public transmitter
{
};

