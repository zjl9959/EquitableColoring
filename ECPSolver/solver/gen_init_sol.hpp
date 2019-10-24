#ifndef ECPSOLVER_GEN_INIT_SOL_H
#define ECPSOLVER_GEN_INIT_SOL_H

#include "../data.hpp"

namespace solver {

void gen_equitable_solution(const Input &input, Solution &sol) {
    // [TODO][TEST]
    int small_color_size = input.graph.nb_node / input.nb_color;
    int big_color_size = small_color_size + 1;
    int nb_big_color = input.graph.nb_node % input.nb_color;
    int nb_small_color = input.nb_color - nb_big_color;
    List<int> nodes(input.graph.nb_node, INVALID);
    for (int i = 0; i < input.graph.nb_node; ++i)
        nodes[i] = i;
    // random partition nodes.
    for (int i = 0; i < small_color_size * nb_small_color; ++i) {
        int rand_num = input.rand.genInt(input.graph.nb_node - i);
        std::swap(nodes[rand_num], nodes[input.graph.nb_node - i - 1]);
    }
    // assign node into color set.
    for (int i = 0; i < input.graph.nb_node; ++i) {
        if (i < small_color_size * nb_small_color)
            sol.assign(nodes[i], i / small_color_size);
        else
            sol.assign(nodes[i], (i - small_color_size * nb_small_color) / big_color_size + nb_small_color);
    }
}

}

#endif // !ECPSOLVER_GEN_INIT_SOL_H
