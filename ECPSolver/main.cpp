#include "test/test.hpp"
#include "data.hpp"
#include "solver/tabu.hpp"

using namespace std;
using namespace solver;
using namespace zjl_utility;

int main(int argc, char* argv[]) {
    string instance = "DSJC125.1.col";
    Input input(10, 180 * 1000, 23094, instance);
    Solution sol(input.graph.nb_node, input.nb_color);
    for (int i = 0; i < input.graph.nb_node; ++i) {
        sol.assign(i, input.rand.genInt(input.nb_color));
    }
    mylog << "Rand seed:" << input.rand.getSeed() << logsw_info;
    TabuSearch solver(input, sol);
    cout << solver.run();
    system("pause");
    return 0;
}
