#include "test/test.hpp"
#include "data.hpp"
#include "solver/gen_init_sol.hpp"
#include "solver/tabu.hpp"

using namespace std;
using namespace solver;
using namespace zjl_utility;

int main(int argc, char* argv[]) {
    string instance = "DSJC125.1.col";
    Input input(10, 180 * 1000, 23094, instance);
    Solution sol(input.graph.nb_node, input.nb_color);
    gen_equitable_solution(input, sol);
    mylog << "Rand seed:" << input.rand.getSeed() << logsw_info;
    TabuSearch solver(input, sol);
    cout << solver.run();
    system("pause");
    return 0;
}
