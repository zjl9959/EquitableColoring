#ifndef ECPSOLVER_TABU_H
#define ECPSOLVER_TABU_H

#include "../data.hpp"

namespace solver {

class TabuSearch {
public:
    using Table = List<List<int>>;
    using Moves = List<Move>;

    TabuSearch(const Input &input, const Solution &init_sol);
    bool run();
private:
    void find_move(Moves &moves);
    void make_move(const Moves &moves);
    void get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes, int color = INVALID);
    void update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table);
public:
    int best_obj;
    Solution best_sol;
private:
    int cur_obj_;
    Solution cur_sol_;
    const Input &input_;
    Table conflict_table_;
    Table tabu_table_;
};

}

#endif // !ECPSOLVER_TABU_H
