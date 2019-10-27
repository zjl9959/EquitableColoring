#ifndef ECPSOLVER_TABU_H
#define ECPSOLVER_TABU_H

#include "../data.hpp"

namespace solver {

class TabuSearch {
public:
    using Table = List<List<int>>;

    TabuSearch(const Input &input, const Solution &init_sol);
    bool run();
    const Solution& get_best_sol() const { return best_sol_; }
private:
    struct Info {
        int best_delt = INT_MAX;  // best improvement for one move.
        int iter_steps = 0;
        int nb_total_move = 0;
        int nb_best_move = 0;
        int nb_rand_move = 0;
        int nb_tabu_move = 0;
        const String str() {
            std::ostringstream os;
            os << "iterator steps:" << iter_steps << ";"
                << "best move:" << ((double)nb_best_move / (double)nb_total_move) * 100 << "%;"
                << "rand move:" << ((double)nb_rand_move / (double)nb_total_move) * 100 << "%;"
                << "tabu move:" << ((double)nb_tabu_move / (double)nb_total_move) * 100 << "%;";
            return os.str();
        }
    };
private:
    void ejection_local_search(List<Move> &best_moves) const;
    void get_max_conflict_nodes(const Solution &sol, const Table &conflict_table, List<int> &nodes) const;
    void update_conflict_table(const Solution &sol, const Move &move, Table &conflict_table) const;
    int verify_obj(const Solution &sol) const;
    bool verify_conflict_table(const Solution &sol, const Table &table) const;
private:
    Info info_;
    int best_obj_, cur_obj_;
    Solution best_sol_, cur_sol_;
    const Input &input_;
    Table conflict_table_;
    Table tabu_table_;
};

}

#endif // !ECPSOLVER_TABU_H
