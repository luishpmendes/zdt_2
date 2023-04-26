#pragma once

#include "solver/solver.hpp"

namespace zdt {
/****************************************************
 * The NSGA2_Solver represents a solver for the ZDT
 * test functions using the Improved Harmony Search.
 ****************************************************/
class IHS_Solver : public Solver {
    public:
    /***************************************
     * Probability of choosing from memory.
     ***************************************/
    double phmcr = 0.85;

    /*********************************
     * Minimum pitch adjustment rate.
     *********************************/
    double ppar_min = 0.35;

    /*********************************
     * Maximum pitch adjustment rate.
     *********************************/
    double ppar_max = 0.99;

    /******************************
     * Minimum distance bandwidth.
     ******************************/
    double bw_min = 1E-5;

    /******************************
     * Maximum distance bandwidth.
     ******************************/
    double bw_max = 1.0;

    /***********************************************************
     * Constructs a new solver.
     *
     * @param zdt the id of the ZDT test function to be solved.
     ***********************************************************/
    IHS_Solver(unsigned zdt);

    /******************************
     * Constructs an empty solver.
     ******************************/
    IHS_Solver();

    /**********************
     * Solve the instance.
     **********************/
    void solve();

    /***************************************************************
     * Standard stream operator.
     *
     * @param os the standard output stream object.
     * @param solver the solver.
     *
     * @return the stream object.
     ***************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const IHS_Solver & solver);
};

}
