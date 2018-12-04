#include "phase_singularity.h"
#include <cmath>

void phase_singularity_map::line_integral(phasemap& phases)
{
    double value;
    data.zeros(phases.x_dim(), phases.y_dim());

    for(int i=2; i<phases.x_dim()-2; i+=1)
    {
        for(int j=2; j<phases.y_dim()-2; j+=1)
        {
            for(unsigned int k=0; k<iter.size(); k++)
            { 
                if(k == iter.size()-1)
                    value = phases.val(i+iter[0].first, j + iter[0].second)-phases.val(i+iter[k].first, j + iter[k].second);
                else
                    value = phases.val(i+iter[k+1].first, j + iter[k+1].second)-phases.val(i+iter[k].first, j + iter[k].second);	

                if(value > PI || value < -PI)
                {
                    value = value - 2*PI* std::floor((value + PI)/(2*PI) );
                }
                if( value < -PI+0.5)
                    data(i,j) -= 1;
                else if(value > PI-0.5)
                    data(i,j) += 1;
            }
        }
    }
}


int phase_singularity_map::count_singularitys()
{
    int num = arma::accu(arma::abs(data));
    return num;
}

