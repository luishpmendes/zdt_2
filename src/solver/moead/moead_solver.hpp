#pragma once

#include "solver/solver.hpp"

namespace zdt {
/************************************************************************
 * The MOEAD_Solver represents a solver for the ZDT test functions using
 * the Multi-objective Evolutionary Algorithm by Decomposition.
 ************************************************************************/
class MOEAD_Solver : public Solver {
    public:
    /********************************************
     * Method used to generate the weights, one
     * of “grid”, “low discrepancy” or “random”.
     ********************************************/
    std::string weight_generation = "random";

    /******************************************************************
     * Decomposition method: one of “weighted”, “tchebycheff” or “bi”.
     ******************************************************************/
    std::string decomposition = "tchebycheff";

    /*************************************
     * Size of the weight’s neighborhood.
     *************************************/
    unsigned neighbours = 20;

    /**************************************************************
     * Crossover parameter in the Differential Evolution operator.
     **************************************************************/
    double cr = 1.0;

    /*****************************************************
     * Parameter for the Differential Evolution operator.
     *****************************************************/
    double f = 0.5;

    /******************************************************
     * Distribution index used by the polynomial mutation.
     ******************************************************/
    double eta_m = 20.0;

    /*************************************************************************
     * Chance that the neighbourhood is considered at each generation,
     * rather than the whole population (only if preserve_diversity is true).
     *************************************************************************/
    double realb = 0.9;

    /********************************************************
     * Maximum number of copies reinserted in the population
     * (only if m_preserve_diversity is true).
     ********************************************************/
    unsigned limit = 2;

    /***********************************************************
     * When true activates the two diversity preservation
     * mechanisms described in Li, Hui, and Qingfu Zhang paper.
     ***********************************************************/
    bool preserve_diversity = true;

    /***********************************************************
     * Constructs a new solver.
     *
     * @param zdt the id of the ZDT test function to be solved.
     ***********************************************************/
    MOEAD_Solver(unsigned zdt);

    /******************************
     * Constructs an empty solver.
     ******************************/
    MOEAD_Solver();

    /***********************
     * Solves the instance.
     ***********************/
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
                                      const MOEAD_Solver & solver);
};

}
