#include "test/test.hpp"
#include "data.hpp"
#include "solver/gen_init_sol.hpp"
#include "solver/tabu.hpp"

using namespace std;
using namespace solver;
using namespace zjl_utility;

int main(int argc, char* argv[]) {
    string instance = "DSJC125.1.col";
    // instance = "test/test_n8e11.col";
    Input input(5, 180 * 1000, 23094, instance);
    Solution sol(input.graph.nb_node, input.nb_color);
    //Solution sol(input.env.solution_dir() + "init_sol/test_n8e11.txt", input.graph.node_id_map);
    gen_equitable_solution(input, sol);
    mylog << "Rand seed:" << input.rand.getSeed() << logsw_info;
    TabuSearch solver(input, sol);
    bool res = solver.run();
    if (res)
        cout << "Find solution." << endl;
    else
        cout << "Not find solution." << endl;
    system("pause");
    return 0;
}
