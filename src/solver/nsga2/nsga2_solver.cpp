#include "solver/nsga2/nsga2_solver.hpp"
#include <pagmo/problem.hpp>
#include <pagmo/problems/zdt.hpp>
#include <pagmo/algorithms/nsga2.hpp>

namespace zdt {

NSGA2_Solver::NSGA2_Solver(unsigned zdt) : Solver::Solver(zdt) {}

NSGA2_Solver::NSGA2_Solver() = default;

void NSGA2_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    pagmo::problem prob{pagmo::zdt(this->zdt, this->m)};

    pagmo::algorithm algo{pagmo::nsga2(1,
                                       this->crossover_probability,
                                       this->crossover_distribution,
                                       this->mutation_probability,
                                       this->mutation_distribution,
                                       this->seed)};

    if(this->max_num_snapshots > 0) {
        this->time_between_snapshots = this->time_limit /
            this->max_num_snapshots;
        this->iterations_between_snapshots = this->iterations_limit /
            this->max_num_snapshots;
    }

    pagmo::population pop{prob, this->population_size, this->seed};

    this->update_best_individuals(pop);

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot(pop);
    }

    while(!this->are_termination_criteria_met()) {
        this->num_iterations++;
        pop = algo.evolve(pop);
        this->update_best_individuals(pop);

        if(this->max_num_snapshots > 0 &&
           this->num_snapshots < this->max_num_snapshots &&
          (this->elapsed_time() - this->time_last_snapshot >=
           this->time_between_snapshots ||
           this->num_iterations - this->iteration_last_snapshot >=
           this->iterations_between_snapshots)) {
            this->capture_snapshot(pop);
        }
    }

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot(pop);
    }

    this->solving_time = this->elapsed_time();
}

std::ostream & operator <<(std::ostream & os, const NSGA2_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Crossover probability: " << solver.crossover_probability << std::endl
       << "Distribution index for crossover: " << solver.crossover_distribution
       << std::endl
       << "Mutation probability: " << solver.mutation_probability << std::endl
       << "Distribution index for mutation: " << solver.mutation_distribution
       << std::endl;
    return os;
}

}
