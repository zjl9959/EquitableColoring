#include "tabu.hpp"

using namespace std;
using namespace zjl_utility;

namespace solver {

TabuSearch::TabuSearch(const Input &input, const Solution &init_sol) : input_(input), cur_sol_(init_sol), best_sol(init_sol) {
    if (init_sol.node_color.size() != input_.graph.nb_node)
        throw "bad init_sol!";
    // init conflict table
    conflict_table_.resize(input_.graph.nb_node, List<int>(input_.nb_color, 0));
    for (int n = 0; n < input_.graph.nb_node; ++n) {
        for (int node : input_.graph.neighbors[n]) {
            conflict_table_[n][init_sol.node_color[node]]++;
        }
    }
    // init cur_sol_, best_sol
    cur_obj_ = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i)
        cur_obj_ += conflict_table_[i][cur_sol_.node_color[i]];
    best_sol = cur_sol_;
    // [zjl][TODO] : init tabu_table
}

bool TabuSearch::run() {
    return false;
}

void TabuSearch::find_move(Moves &moves) {
    Table copy_conflict_table = conflict_table_;
    Solution copy_sol = cur_sol_;
    int obj = cur_obj_;
    int chain_length = 0;   // ejection chain length.
    List<Move> branch_moves; // moves to be explored.
    while (!branch_moves.empty()) {
        // [zjl][TODO]
    }
}

void TabuSearch::make_move(const Moves &moves) {
    // [zjl][TODO]
}

void TabuSearch::get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes, int color) {
    int max_conflict = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i)
        if (sol.node_color(i) == color)
            max_conflict = max(max_conflict, conflict_table[i][sol.node_color(i)]);
    for (int i = 0; i < input_.graph.nb_node; ++i)
        if (conflict_table[i][sol.node_color(i)] == max_conflict && sol.node_color(i) == color)
            nodes.push_back(i);
}

void TabuSearch::update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table) {
    for (int node : input_.graph.neighbors[move.node]) {
        if (sol.node_color(node) == move.old_color)
            conflict_table[node][move.old_color]--;
        if (sol.node_color(node) == move.new_color)
            conflict_table[node][move.new_color]++;
    }
}

}
