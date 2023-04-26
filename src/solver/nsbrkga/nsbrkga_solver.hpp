#pragma once

#include "solver/solver.hpp"
#include "solver/nsbrkga/decoder.hpp"

namespace zdt {
/***********************************************************************
 * The NSBRKGA_Solver represents a solver for the ZDT test functions
 * using the Non-Dominated Sorting Biased Random-Key Genetic Algorithm.
 ***********************************************************************/
class NSBRKGA_Solver : public Solver {
    public:
    /********************************************
     * Number of individuals in each population.
     ********************************************/
    unsigned population_size = 1000;

    /********************************************************************
     * Minimum percentage of individuals to become the elite set (0, 1].
     ********************************************************************/
    double min_elites_percentage = 0.10;

    /********************************************************************
     * Maximum percentage of individuals to become the elite set (0, 1].
     ********************************************************************/
    double max_elites_percentage = 0.50;

    /***********************************
     * Mutation probability.
     ***********************************/
    double mutation_probability = 0.01;

    /************************************
     * Mutation distribution.
     ************************************/
    double mutation_distribution = 50.0;

    /**************************************
     * Number of total parents for mating.
     **************************************/
    unsigned num_total_parents = 3;

    /**************************************
     * Number of elite parents for mating.
     **************************************/
    unsigned num_elite_parents = 2;

    /************************************************************************
     * Type of bias that will be used.
     ************************************************************************/
    BRKGA::BiasFunctionType bias_type = BRKGA::BiasFunctionType::LOGINVERSE;

    /***************************************************************
     * Type of diversity that will be used.
     ***************************************************************/
    BRKGA::DiversityFunctionType diversity_type =
        BRKGA::DiversityFunctionType::AVERAGE_DISTANCE_TO_CENTROID;

    /**********************************************
     * Number of independent parallel populations.
     **********************************************/
    unsigned num_populations = 1;

    /*********************************************************************
     * Number of pairs of chromosomes to be tested to path-relinking.
     *********************************************************************/
    unsigned pr_number_pairs = 100;

    /*******************************************************************
     * Minimum distance between chromosomes selected to path-relinking.
     *******************************************************************/
    double pr_min_dist = 0.15;

    /**************************************************
     * Individual selection to path-relinking.
     **************************************************/
    BRKGA::PathRelinking::Selection pr_selection =
        BRKGA::PathRelinking::Selection::BESTSOLUTION;

    /*****************************************
     * Percentage of the path to be computed.
     *****************************************/
    double pr_percentage = 0.30;

    /***********************************************
     * Interval at which the path relink is applied
     * (0 means no path relinking).
     ***********************************************/
    unsigned pr_interval = 200;

    /*********************************************************************
     * Interval at which the populations are shaken (0 means no shaking).
     *********************************************************************/
    unsigned shake_interval = 200;

    /********************************
     * The intensity of the shaking.
     ********************************/
    unsigned shake_intensity = 10;

    /******************************************************************
     * Interval at which the populations are reset (0 means no reset).
     ******************************************************************/
    unsigned reset_interval = 500;

    /******************************
     * The intensity of the reset.
     ******************************/
    double reset_intensity = 0.5;

    /*********************************************************
     * Number of threads to be used during parallel decoding.
     *********************************************************/
    unsigned num_threads = 1;

    /*********************************************************
     * The maximum number of local search iterations allowed.
     *********************************************************/
    unsigned max_local_search_iterations = 0;

    /************************************
     * Last update generation.
     ************************************/
    unsigned last_update_generation = 0;

    /******************************
     * Last update time.
     ******************************/
    double last_update_time = 0.0;

    /******************************************************
     * Largest number of generations between improvements.
     ******************************************************/
    unsigned large_offset = 0;

    /******************************
     * Total path relink time.
     ******************************/
    double path_relink_time = 0.0;

    /***********************************
     * Total path relink calls.
     ***********************************/
    unsigned num_path_relink_calls = 0;

    /*******************************
     * Number of homogeneities.
     *******************************/
    unsigned num_homogeneities = 0;

    /***********************************
     * Improvements in the elite set.
     ***********************************/
    unsigned num_elite_improvments = 0;

    /***********************************
     * Best individual improvements.
     ***********************************/
    unsigned num_best_improvements = 0;

    /**************************
     * Total shaking calls.
     **************************/
    unsigned num_shakings = 0;

    /************************
     * Total reset calls.
     ************************/
    unsigned num_resets = 0;

    /******************************************************************
     * Snapshots of the number of elite individuals,
     * containing the iteration, time and number of elite individuals.
     ******************************************************************/
    std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
        num_elites_snapshots = {};

    /******************************************************************
     * Number of non-dominated individuals in each current population.
     ******************************************************************/
    std::vector<unsigned> num_non_dominated = {};

    /**********************************************
     * Number of fronts in each current population
     **********************************************/
    std::vector<unsigned> num_fronts = {};

    /**********************************************************
     * Number of elite individuals in each current population.
     **********************************************************/
    std::vector<unsigned> num_elites = {};

    /*************************************************************************
     * The fronts of each current population.
     *************************************************************************/
    std::vector<std::vector<std::pair<std::vector<double>, unsigned>>> fronts
        = {};

    /**********************************************************
     * Constructs a new solver.
     *
     * @param zdt the id of the ZDT test function to be solved
     **********************************************************/
    NSBRKGA_Solver(unsigned zdt);

    /******************************
     * Constructs an empty solver.
     ******************************/
    NSBRKGA_Solver();

    /*******************************************************************
     * Capture a snapshot of the current population.
     *
     * @param pop the current population.
     *******************************************************************/
    void capture_snapshot(const BRKGA::NSBRKGA<Decoder> & algorithm);

    /**********************
     * Solve the instance.
     **********************/
    void solve();

    /*****************************************************************
     * Standard stream operator.
     *
     * @param os the standard output stream object.
     * @param solver the solver.
     *
     * @return the stream object.
     *****************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const NSBRKGA_Solver & solver);
};

}
