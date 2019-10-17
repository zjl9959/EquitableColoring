#ifndef ECPSOLVER_TABU_H
#define ECPSOLVER_TABU_H

#include "../data.hpp"

namespace solver {

class TabuSearch {
public:
    using Table = List<List<int>>;

    TabuSearch(const Input &input, const Solution &init_sol);
    bool run();
private:
    void ejection_local_search();
    void get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes);
    void update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table);
private:
    int best_obj_, cur_obj_;
    Solution best_sol_, cur_sol_;
    const Input &input_;
    Table conflict_table_;
    Table tabu_table_;
};

}

#endif // !ECPSOLVER_TABU_H
