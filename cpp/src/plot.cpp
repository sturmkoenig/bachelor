#include "plot.h"

void plot::AP( std::vector< std::pair<double, double>> AktionPotential)
{
    gp << "set yrange[0:1]\n";
    gp << "set xlabel time \n";
    gp << "set ylabel volage \n";
    gp << "plot '-' w lines\n";
    gp.send1d(AktionPotential);
}

void plot::v_vs_h( std::vector< std::pair<double, double>> VandH)
{
    gp << "reset\n";
    gp << "set xrange [0:1]\n";
    gp << "set yrange[0:1]\n";
    gp << "set xlabel time \n";
    gp << "set ylabel volage \n";
    gp << "plot '-' w lines\n";
    gp.send1d(VandH);
}

void plot::phasemap(arma::mat& pm, double* range)
{
    gp << "set cbrange [" << range[0] << ":" << range[1] << "]\n";
    //gp << "set loadpath '../gnuplot/palettes'\n"; 
    //gp << "load 'magma.pal'\n";
    gp << "set xrange [0:" << Lx-1 << "]\n";
    gp << "set yrange [0:" << Ly-1 << "]\n";
    gp << "set pm3d map\n";
    gp << "unset xtics\n";
    gp << "unset ytics\n";
    gp << "unset key\n";
    gp << "set terminal qt size 700,700\n";
    gp << "splot '-' binary" << gp.binFmt2d(pm, "array") << "with image\n";
    gp.sendBinary2d(pm);
}

void plot::heatmap(arma::mat& A)
{
    double scale = Lx/Ly;
    gp << "set cbrange [0.:1.]\n";
    //gp << "set loadpath '../gnuplot/palettes'\n"; 
    //gp << "load 'magma.pal'\n";
    gp << "set xrange [0:" << 300 << "]\n";
    gp << "set yrange [0:" << 300 << "]\n";
    gp << "set pm3d map\n";
    gp << "unset xtics\n";
    gp << "unset ytics\n";
    gp << "unset key\n";
    gp << "unset colorbox\n";
    // gp << "set terminal qt size "  << int(700./scale) << "," << 700 << "\n";
    gp << "splot '-' binary" << gp.binFmt2d(A, "array") << "with image\n";
    gp.sendBinary2d(A);
}

