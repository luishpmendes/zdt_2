#include "solver/solver.hpp"

namespace zdt {

Solver::Solver(unsigned zdt) : zdt(zdt) {
    this->set_seed(this->seed);
}

Solver::Solver() = default;

void Solver::set_seed(unsigned seed) {
    this->seed = seed;
    this->rng.seed(this->seed);
    this->rng.discard(10000);
}

double Solver::elapsed_time(
        const std::chrono::steady_clock::time_point & start_time) {
    std::chrono::steady_clock::time_point current_time =
        std::chrono::steady_clock::now();
    std::chrono::nanoseconds elapsed_time =
        std::chrono::duration_cast<std::chrono::nanoseconds> (
                current_time - start_time);
    return (double(elapsed_time.count()))/(double(1e9));
}

double Solver::elapsed_time() const {
    return Solver::elapsed_time(this->start_time);
}

double Solver::remaining_time(
        const std::chrono::steady_clock::time_point & start_time,
        double time_limit) {
    return time_limit - Solver::elapsed_time(start_time);
}

double Solver::remaining_time() const {
    return Solver::remaining_time(this->start_time, this->time_limit);
}

bool Solver::are_termination_criteria_met() const {
    return (this->elapsed_time() >= this->time_limit ||
            this->num_iterations >= this->iterations_limit);
}

bool Solver::dominates(const std::vector<double> & valueA,
                       const std::vector<double> & valueB) {
    if(valueA.size() != valueB.size()) {
        return false;
    }

    // Checks if valueA is at least as good as valueB
    for(unsigned i = 0; i < valueA.size(); i++) {
        if(valueA[i] > valueB[i] + std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    // Checks if valueA is better than valueB
    for(unsigned i = 0; i < valueA.size(); i++) {
        if(valueA[i] < valueB[i] - std::numeric_limits<double>::epsilon()) {
            return true;
        }
    }

    return false;
}

bool Solver::update_best_individuals(
            std::vector<std::pair<std::vector<double>, std::vector<double>>> & best_individuals,
            const std::vector<
                std::pair<std::vector<double>, std::vector<double>>> & new_individuals,
            const std::vector<BRKGA::Sense> & senses) {
    bool result = false;

    if(new_individuals.empty()) {
        return result;
    }

    auto non_dominated_new_individuals =
        BRKGA::Population::nonDominatedSort<std::vector<double>>(
                new_individuals,
                senses).front();

    for(const auto & new_individual : non_dominated_new_individuals) {
        bool is_dominated_or_equal = false;

        for(auto it  = best_individuals.begin();
                 it != best_individuals.end();) {
            auto individual = *it;

            if(Solver::dominates(new_individual.first, individual.first)) {
                it = best_individuals.erase(it);
            } else {
                if(Solver::dominates(individual.first,
                                     new_individual.first)
                        || std::equal(individual.first.begin(),
                                      individual.first.end(),
                                      new_individual.first.begin(),
                                      [](const double & a, const double & b) {
                                            return fabs(a - b) <
                                                std::numeric_limits<double>::epsilon();
                                      })) {
                    is_dominated_or_equal = true;
                    break;
                }

                it++;
            }
        }

        if(!is_dominated_or_equal) {
            best_individuals.push_back(new_individual);
            result = true;
        }
    }

    return result;
}

bool Solver::update_best_individuals(
            const std::vector<
                std::pair<std::vector<double>,
                          std::vector<double>>> & new_individuals) {
    bool result = Solver::update_best_individuals(
            this->best_individuals,
            new_individuals,
            this->senses);

    if(this->best_individuals.size() > this->max_num_solutions) {
        BRKGA::Population::crowdingSort<std::vector<double>>(
                this->best_individuals,
                this->rng);
        this->best_individuals.resize(this->max_num_solutions);
        result = true;
    }

    return result;
}

bool Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return this->update_best_individuals(new_individuals);
}

void Solver::capture_snapshot(const pagmo::population & pop) {
    double time_snapshot = this->elapsed_time();

    this->best_solutions_snapshots.emplace_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<std::vector<double>>(
                    this->best_individuals.size())));
    for(unsigned i = 0; i < this->best_individuals.size(); i++) {
        std::get<2>(this->best_solutions_snapshots.back())[i] =
            this->best_individuals[i].first;
    }

    this->current_individuals.resize(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        this->current_individuals[i] = std::make_pair(pop.get_f()[i],
                                                      pop.get_x()[i]);
    }

    this->fronts = BRKGA::Population::nonDominatedSort<std::vector<double>>(
            current_individuals,
            this->senses);

    this->num_non_dominated_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.front().size())));

    this->num_fronts_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.size())));

    this->populations_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<std::vector<std::vector<double>>>(1, pop.get_f())));

    this->time_last_snapshot = time_snapshot;
    this->iteration_last_snapshot = this->num_iterations;
    this->num_snapshots++;
}

std::ostream & operator <<(std::ostream & os, const Solver & solver) {
    os << "ZDT: " << solver.zdt << std::endl
       << "Number of variables: " << solver.m << std::endl
       << "Seed: " << solver.seed << std::endl
       << "Time limit: " << solver.time_limit << std::endl
       << "Iterations limit: " << solver.iterations_limit << std::endl
       << "Maximum number of solutions: " << solver.max_num_solutions
       << std::endl
       << "Maximum number of snapshots: " << solver.max_num_snapshots
       << std::endl
       << "Population size: " << solver.population_size << std::endl
       << "Number of iterations: " << solver.num_iterations << std::endl
       << "Solutions obtained: " << solver.best_individuals.size() << std::endl
       << "Solving time: " << solver.solving_time << std::endl
       << "Number of snapshots: " << solver.num_snapshots << std::endl
       << "Time between snapshots: " << solver.time_between_snapshots
       << std::endl
       << "Time when the last snapshot was taken: "
       << solver.time_last_snapshot << std::endl
       << "Number of iterations between the snapshots: "
       << solver.iterations_between_snapshots << std::endl
       << "Iteration when the last snapshot was taken: "
       << solver.iteration_last_snapshot << std::endl;
    return os;
}

}
