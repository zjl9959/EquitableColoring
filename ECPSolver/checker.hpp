#ifndef ECPSOLVER_CHECKER_H
#define ECPSOLVER_CHECKER_H

#include "data.hpp"
#include "utils/utility.hpp"

namespace solver {

bool checker_solution(const Input &input, const Solution &sol) {
    bool valid_solution = true;
    UMap<int, int> counter;
    // check node assign color.
    for (int i = 0; i < input.graph.nb_node; ++i) {
        if (sol[i] < 0 || sol[i] >= input.nb_color) {
            zjl_utility::mylog << "Solution node " << i
                << " has invalid color " << sol[i] << zjl_utility::logsw_error;
            valid_solution = false;
        } else {
            if (counter.find(sol[i]) != counter.end()) {
                counter[sol[i]]++;
            } else {
                counter[sol[i]] = 0;
            }
        }
    }
    // check color equitable.
    int min_color_size = input.graph.nb_node, max_color_size = 0;
    for (auto it = counter.begin(); it != counter.end(); ++it) {
        min_color_size = std::min(min_color_size, it->second);
        max_color_size = std::max(max_color_size, it->second);
    }
    if (max_color_size - min_color_size > 1) {
        zjl_utility::mylog << "Violate equitable constraint." << zjl_utility::logsw_error;
        valid_solution = false;
    }
    // check edge conflict.
    for (int i = 0; i < input.graph.nb_node; ++i) {
        for (int neighbor : input.graph.neighbors[i]) {
            if (sol[i] == sol[neighbor]) {
                zjl_utility::mylog << "Node " << i << " and node "
                    << neighbor << " conflict." << zjl_utility::logsw_error;
                valid_solution = false;
            }
        }
    }
    return valid_solution;
}

}

#endif // !ECPSOLVER_CHECKER_H
