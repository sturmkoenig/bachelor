#include <stdexcept>
#include <experimental/random>

#include "perturbation.h"

// default constructor
perturbation::perturbation()
{
    t=x=y=d=f=0;
    return;
}

perturbation::perturbation(int time, int posx, int posy, int drnt, int frqn)
{
    // discritption of parameters in header 
    t = time;
    x = posx;
    y = posy; 
    d = drnt;
    f = frqn;

    // incase of out of bound parameters throw error
    if(x<0 || x>MAT_SIZE || y<0 || y>MAT_SIZE)
        throw std::invalid_argument("perturbation position out of bounds ");
    return;
}


void perturbation::force(matrix& a, int time)
{
    // perturbate until t
    if(t>time)
    {
        // for duration d perturb system
        //for (int i = 0; i < d; i++)
        //{
            // controll if time is in the intervall (f, f+d)
            //if ((time+i)%f == 0) 
            //{
                a(x,y) += 0.2; 
            //}
		
			
        //}
	//if((time)%f == 0)
	//	{
        //		// optionally randomize perturbation postion between [1,L-1]
        //		x = std::experimental::randint(1,MAT_SIZE-1);
        //		y = std::experimental::randint(1,MAT_SIZE-1);
	//	}
    }
    return;
}


