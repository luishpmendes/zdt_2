#pragma once

#define NSBRKGA_MULTIPLE_INCLUSIONS

#include "nsbrkga.hpp"
#include <chrono>
#include <random>
#include <vector>
#include <pagmo/population.hpp>

namespace zdt {
/****************************************************************************
 * The Solver abstract class represents a solver for the ZDT test functions.
 ****************************************************************************/
class Solver {
    public:

    /***********************************************
     * The id of the ZDT test function been solved.
     ***********************************************/
    unsigned zdt;

    /***************************
     * The number of variables.
     ***************************/
    unsigned m = 30;

    /***************************************************************
     * The optimisation senses.
     ***************************************************************/
    std::vector<BRKGA::Sense> senses = std::vector<BRKGA::Sense>(2,
        BRKGA::Sense::MINIMIZE);

    /****************************************************************
     * The seed for the pseudo-random numbers generator.
     ****************************************************************/
    unsigned seed =
        std::chrono::system_clock::now().time_since_epoch().count();

    /**************************************
     * The pseudo-random number generator.
     **************************************/
    std::mt19937 rng;

    /*******************************************************
     * The time limit in seconds.
     *******************************************************/
    double time_limit = std::numeric_limits<double>::max();

    /*****************************************************************
     * The iterations limit.
     *****************************************************************/
    unsigned iterations_limit = std::numeric_limits<unsigned>::max();

    /******************************************************************
     * The maximum number of solutions.
     ******************************************************************/
    unsigned max_num_solutions = std::numeric_limits<unsigned>::max();

    /***********************************************************
     * Maximum number of snapshots to take during optimization.
     ***********************************************************/
    unsigned max_num_snapshots = 0;

    /*******************************
     * Size of the population.
     *******************************/
    unsigned population_size = 100;

    /*************************************
     * The number of iterations executed.
     *************************************/
    unsigned num_iterations = 0;

    /****************************************************************
     * The best individuals found.
     ****************************************************************/
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        best_individuals;

    /*******************************
     * The solving time in seconds.
     *******************************/
    double solving_time = 0.0;

    /*************************************************
     * Number of snapshots taken during optimization.
     *************************************************/
    unsigned num_snapshots = 0;

    /************************************
     * The time between the snapshots.
     ************************************/
    double time_between_snapshots = 0.0;

    /*********************************************
     * The time when the last snapshot was taken.
     *********************************************/
    double time_last_snapshot = 0.0;

    /**************************************************
     * The number of iterations between the snapshots.
     **************************************************/
    unsigned iterations_between_snapshots = 0;

    /**************************************************
     * The iteration when the last snapshot was taken.
     **************************************************/
    unsigned iteration_last_snapshot = 0;

    /**********************************************************************
     * Snapshots of the best solutions, containing the iteration, time and
     * Solutions' costs.
     **********************************************************************/
    std::vector<std::tuple<unsigned, double,
        std::vector<std::vector<double>>>> best_solutions_snapshots = {};

    /*****************************************************************
     * Snapshots of the number of non-dominated individuals in each
     * population, containing the iterations, time and number of
     * non-dominated individuals in each population.
     *****************************************************************/
    std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
        num_non_dominated_snapshots = {};

    /***********************************************************************
     * Snapshots of the number of non-dominated fronts in each population,
     * containing the iterations, time and number of non-dominated fronts in
     * each population.
     ***********************************************************************/
    std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
        num_fronts_snapshots = {};

    /*******************************************************************
     * Snapshots of the populations, containing the iteration, time and
     * Solutions' costs.
     *******************************************************************/
    std::vector<std::tuple<unsigned, double,
        std::vector<std::vector<std::vector<double>>>>>
            populations_snapshots = {};

    /*************************************************
     * The start time.
     *************************************************/
    std::chrono::steady_clock::time_point start_time;

    /****************************************************************
     * The current individuals.
     ****************************************************************/
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        current_individuals;

    /****************************************************************
     * The current fronts.
     ****************************************************************/
    std::vector<std::vector<std::pair<std::vector<double>,
                                      std::vector<double>>>> fronts;

    /**********************************************************
     * Constructs a new solver.
     *
     * @param zdt the id of the ZDT test function been solved.
     **********************************************************/
    Solver(unsigned zdt);

    /******************************
     * Constructs an empty solver.
     ******************************/
    Solver();

    /********************************************************************
     * Sets the seed for the pseudo-random numbers generator.
     *
     * @param seed the new seed for the pseudo-random numbers generator.
     ********************************************************************/
    void set_seed(unsigned seed);

    /******************************************************************
     * Returns the elapsed time in seconds.
     *
     * @param start_time the start time
     *
     * @return the elapsed time in seconds.
     ******************************************************************/
    static double elapsed_time(
            const std::chrono::steady_clock::time_point & start_time);

    /***************************************
     * Returns the elapsed time in seconds.
     *
     * @return the elapsed time.
     ***************************************/
    double elapsed_time() const;

    /****************************************************************
     * Returns the remaining time in seconds.
     *
     * @param start_time the start time.
     * @param time_limit the time limit in seconds.
     *
     * @return the remaining time.
     ****************************************************************/
    static double remaining_time(
            const std::chrono::steady_clock::time_point & start_time,
            double time_limit);

    /*****************************************
     * Returns the remaining time in seconds.
     *
     * @return the remaining time in seconds.
     *****************************************/
    double remaining_time() const;

    /***********************************************************
     * Verifies whether the termination criteria have been met.
     *
     * @return true if the termination criteria have been met;
     *         false otherwise.
     ***********************************************************/
    bool are_termination_criteria_met() const;

    /************************************************************
     * Returns true if valueA dominates valueB; false otherwise.
     *
     * @param valueA the first cost been compared.
     * @param valueB the second cost been compared.
     *
     * @return true if valueA dominates valueB; false otherwise.
     ************************************************************/
    static bool dominates(const std::vector<double> & valueA,
                          const std::vector<double> & valueB);

    /*********************************************************************
     * Update the best individuals found so far.
     *
     * @param best_individuals the best individuals found so far.
     * @param new_individuals  the new individuals found.
     *
     * @return true if the best individual are modified; false otherwise.
     *********************************************************************/
    static bool update_best_individuals(
            std::vector<std::pair<std::vector<double>,
                                  std::vector<double>>> & best_individuals,
            const std::vector<
                std::pair<std::vector<double>,
                          std::vector<double>>> & new_individuals,
            const std::vector<BRKGA::Sense> & senses);

    /*********************************************************************
     * Update the best individuals found so far.
     *
     * @param new_individuals the new individuals found.
     *
     * @return true if the best individual are modified; false otherwise.
     *********************************************************************/
    bool update_best_individuals(
            const std::vector<
                std::pair<std::vector<double>,
                          std::vector<double>>> & new_individuals);

    /**********************************************************************
     * Update the best individuals found so far.
     *
     * @param pop the new solutions.
     *
     * @return true if the best individuals are modified; false otherwise.
     **********************************************************************/
    bool update_best_individuals(const pagmo::population & pop);

    /*****************************************************
     * Capture a snapshot of the current population.
     *
     * @param pop the current population.
     *****************************************************/
    void capture_snapshot(const pagmo::population & pop);

    /*************************
     * Solves the instance.
     *************************/
    virtual void solve() = 0;

    /*********************************************************
     * Standard stream operator.
     *
     * @param os the standard output stream object.
     * @param solver the solver.
     *
     * @return the stream object.
     *********************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Solver & solver);

};

}
