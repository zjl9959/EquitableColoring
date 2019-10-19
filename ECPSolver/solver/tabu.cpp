#include "tabu.hpp"

using namespace std;
using namespace zjl_utility;

namespace solver {

TabuSearch::TabuSearch(const Input &input, const Solution &init_sol) : input_(input), cur_sol_(init_sol), best_sol_(init_sol) {
    // init conflict table
    conflict_table_.resize(input_.graph.nb_node, List<int>(input_.nb_color, 0));
    for (int n = 0; n < input_.graph.nb_node; ++n) {
        for (int node : input_.graph.neighbors[n]) {
            conflict_table_[n][init_sol[node]]++;
        }
    }
    // init cur_sol_, best_sol
    cur_obj_ = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i)
        cur_obj_ += conflict_table_[i][cur_sol_[i]];
    best_obj_ = cur_obj_;
    // [zjl][TODO] : init tabu_table
}

bool TabuSearch::run() {
    while (!input_.timer.isTimeOut()) {
        // TODO
    }
    return false;
}

void TabuSearch::ejection_local_search(List<Move> &best_moves) {
    int best_obj = cur_obj_;
    Table Ct = conflict_table_; // Ct: copy of conflict table.
    Solution S = cur_sol_;  // S: current solution.
    List<int> max_conflict_nodes;
    get_max_conflict_nodes(S, Ct, max_conflict_nodes);
    for (int node : max_conflict_nodes) {
        int obj = cur_obj_;
        obj += update_conflict_table(S, Move(node, S[node], INVALID), Ct);
        int min_set_size = S.color_size(node);
        int trial_color = S[node];   // the color where ejected node finally will be put back.
        int chain_length = 0;   // ejection chain length.
        S.update(node, INVALID);
        List<Move> move_chain;  // current ejection chain iteration moves.
        while (chain_length < input_.cfg.max_chain_length) {
            // try to put the node into trial color and update.[not really put the node back]
            int delt = 0;
            for (int neighbor : input_.graph.neighbors[node]) {
                if (S[neighbor] == trial_color)
                    ++delt;
            }
            if (obj + delt < best_obj) {
                best_obj = obj + delt;
                best_moves = move_chain;
            }
            // put other color set's most conflict node into this trail_color.
            for (int n = 0; n < input_.graph.nb_node; ++n) {
                
            }
        }
    }
}

/*
 * INPUT (sol: solution to be consider; conflict_table: conflict_table for this solution).
 * OUTPUT (nodes: nodes with most conflict edges.)
*/
void TabuSearch::get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes) {
    int max_conflict = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i)
            max_conflict = max(max_conflict, conflict_table[i][sol[i]]);
    for (int i = 0; i < input_.graph.nb_node; ++i)
        if (conflict_table[i][sol[i]] == max_conflict)
            nodes.push_back(i);
}

int TabuSearch::update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table) {
    int delt = 0;
    for (int node : input_.graph.neighbors[move.node]) {
        if (sol[node] == move.old_color) {
            conflict_table[node][move.old_color]--;
            --delt;
        }
        if (sol[node] == move.new_color) {
            conflict_table[node][move.new_color]++;
            ++delt;
        }
    }
    return delt;
}

}
