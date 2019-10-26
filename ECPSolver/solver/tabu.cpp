#include "tabu.hpp"
#include <assert.h>

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
    assert(cur_obj_ % 2 == 0);
    cur_obj_ /= 2;
    best_obj_ = cur_obj_;
    // init tabu table.
    cur_iter_ = 0;
    best_delt_ = input_.graph.nb_edge;
    tabu_table_.resize(input.graph.nb_node, List<int>(input.nb_color, 0));
}

bool TabuSearch::run() {
    int no_improve_iter = 0;
    while (!input_.timer.isTimeOut()) {
        List<Move> best_moves;
        ejection_local_search(best_moves);  // find move.
        assert(best_moves.size() <= input_.cfg.max_chain_length);
        // make move and update cur_obj_.
        int delt = 0;
        for (auto it = best_moves.begin(); it != best_moves.end(); ++it) {
            delt += conflict_table_[it->node][it->new_color] - conflict_table_[it->node][it->old_color];
            cur_sol_.update(it->node, it->new_color);
            update_conflict_table(cur_sol_, *it, conflict_table_);
            best_delt_ = min(best_delt_, delt); // update best delt.
            tabu_table_[it->node][it->old_color] =
                cur_iter_ + input_.nb_color + input_.rand.genInt(input_.nb_color);  // update tabu table.
        }
        cur_obj_ += delt;
        int obj_verify = verify_obj();
        assert(cur_obj_ == obj_verify); // check objective match.
        // update best_sol_.
        if (cur_obj_ < best_obj_) {
            best_obj_ = cur_obj_;
            best_sol_ = cur_sol_;
            mylog << "A better obj:" << best_obj_ << logsw_info;
        }
        // check best_obj_'s equitable.
        if (best_obj_ == 0) {
            return true;
        }
        if (delt < 0) {
            no_improve_iter = 0;
        } else {
            ++no_improve_iter;
            // [zjl][TODO]: add some perturbation.
        }
        ++cur_iter_;
    }
    return false;
}

/*
 * INPUT: use member variables. (cur_sol_:current solution; conflict_table_: current conflict table.)
 * OUTPUT: (best_moves: best moves found after one iterator of ejection chain;
 * RETURN: (current solution's objective after make best moves.)
*/
void TabuSearch::ejection_local_search(List<Move> &best_moves) const {
    int best_obj = input_.graph.nb_edge;
    List<int> max_conflict_nodes;
    get_max_conflict_nodes(cur_sol_, conflict_table_, max_conflict_nodes);
    for (int node : max_conflict_nodes) {
        Table Ct = conflict_table_; // Ct: copy of conflict table.
        Solution S = cur_sol_;  // S: current solution.
        int obj = cur_obj_ - Ct[node][S[node]];
        update_conflict_table(S, Move(node, S[node], INVALID), Ct);
        int ejected_node_color = S[node];
        int trial_color = S[node];   // the color where ejected node finally will be put back.
        int chain_length = 0;   // ejection chain length.
        S.remove(node);
        List<Move> move_chain;  // current ejection chain iteration moves.
        while (chain_length < input_.cfg.max_chain_length) {
            // try to put the node into trial color and update.[not really put the node back]
            int delt = conflict_table_[node][trial_color];
            if (obj + delt < best_obj) {
                best_obj = obj + delt;
                best_moves = move_chain;
                best_moves.push_back(Move(node, ejected_node_color, trial_color));
            }
            // find best move2(move2: ejection one node into trial color).
            int pool_sampling_count = 0, pool_sampling_count_tabu = 0;
            int best_move2_delt = input_.graph.nb_edge;    // smaller is better.
            int best_move2_delt_tabu = input_.graph.nb_edge;
            Move best_move2, best_move2_tabu;
            for (int n = 0; n < input_.graph.nb_node; ++n) {
                if (n != node && S[n] != trial_color) {
                    int move2_delt = Ct[n][trial_color] - Ct[n][S[n]];
                    if (tabu_table_[n][trial_color] > cur_iter_) {
                        if (move2_delt < best_move2_delt_tabu || (move2_delt == best_move2_delt_tabu &&
                            input_.rand.genInt(pool_sampling_count_tabu) == 0)) {
                            best_move2_delt_tabu = move2_delt;
                            best_move2_tabu = Move(n, S[n], trial_color);
                            if (move2_delt < best_move2_delt_tabu)
                                pool_sampling_count_tabu = 0;
                            else
                                ++pool_sampling_count_tabu;
                        }
                    }
                    else {
                        if (move2_delt < best_move2_delt || (move2_delt == best_move2_delt &&
                            input_.rand.genInt(pool_sampling_count) == 0)) {
                            best_move2_delt = move2_delt;
                            best_move2 = Move(n, S[n], trial_color);
                            if (move2_delt < best_move2_delt)
                                pool_sampling_count = 0;
                            else
                                ++pool_sampling_count;
                        }
                    }
                }
            }
            // choose the best move2 or random move by some probability.
            Move chosen_move2(best_move2);
            if (best_move2_delt_tabu < best_delt_ && best_move2_delt_tabu < best_move2_delt) {
                chosen_move2 = best_move2_tabu;
            }
            if (best_move2.node == INVALID || input_.rand.genDouble(1.0) > input_.cfg.ejection_greedy_rate) {
                int rand_node = input_.rand.genInt(input_.graph.nb_node);
                chosen_move2 = Move(rand_node, S[rand_node], trial_color);
            }
            // update and record chosen move.
            if (chosen_move2.node == node)  // this should be the trial move, so skip.
                continue;
            obj += conflict_table_[chosen_move2.node][chosen_move2.new_color] -
                conflict_table_[chosen_move2.node][chosen_move2.old_color];
            update_conflict_table(S, chosen_move2, Ct);
            S.update(chosen_move2.node, chosen_move2.new_color);
            move_chain.push_back(chosen_move2);
            trial_color = chosen_move2.old_color;
            ++chain_length;
        }
    }
}

/*
 * INPUT (sol: solution to be consider; conflict_table: conflict_table for this solution).
 * OUTPUT (nodes: nodes with most conflict edges.)
*/
void TabuSearch::get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes) const {
    int max_conflict = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i)
            max_conflict = max(max_conflict, conflict_table[i][sol[i]]);
    for (int i = 0; i < input_.graph.nb_node; ++i)
        if (conflict_table[i][sol[i]] == max_conflict)
            nodes.push_back(i);
}

void TabuSearch::update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table) const {
    for (int node : input_.graph.neighbors[move.node]) {
        if(move.old_color != INVALID)
            conflict_table[node][move.old_color]--;
        if(move.new_color != INVALID)
            conflict_table[node][move.new_color]++;
    }
}

/* RETURN: current solution's objective. */
int TabuSearch::verify_obj() const {
    int ojbk = 0;
    for (int i = 0; i < input_.graph.nb_node; ++i) {
        ojbk += conflict_table_[i][cur_sol_[i]];
    }
    assert(ojbk % 2 == 0);
    ojbk /= 2;
    return ojbk;
}

}
