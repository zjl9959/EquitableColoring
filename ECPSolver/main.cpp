#include "test/test.hpp"
#include "data.hpp"
#include "solver/gen_init_sol.hpp"
#include "solver/tabu.hpp"
#include "checker.hpp"

using namespace std;
using namespace solver;
using namespace zjl_utility;

int main(int argc, char* argv[]) {
    string instance = "DSJC125.1.col";
    Input input(9, 180 * 1000, 23094, instance);
    Solution sol(input.graph.nb_node, input.nb_color);
    gen_equitable_solution(input, sol);
    mylog << "Rand seed:" << input.rand.getSeed() << logsw_info;
    TabuSearch solver(input, sol);
    solver.run();
    if (check_solution(input, solver.get_best_sol())) {
        mylog << "Solution valid !" << logsw_info;
        save_solution(solver.get_best_sol(), input.env.solution_path(), input.graph.node_id_map);
        save_solution(solver.get_best_sol(), input.env.solution_path_withtime(), input.graph.node_id_map);
    } else {
        mylog << "Solution invalid !" << logsw_info;
    }
    system("pause");
    return 0;
}
