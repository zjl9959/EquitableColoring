#include "data.hpp"

using namespace std;
using namespace zjl_utility;

namespace solver {

void load_graph_col(Graph &graph, const String &path);
void load_configure(Configure &cfg, const String &path);

Graph::Graph(const String &path) {
    string suffix = path.substr(path.find_last_of('.'));
    if (suffix == ".col") {
        load_graph_col(*this, path);
    } else {
        throw "Instance type not support!";
    }
}

Configure::Configure(const String &path) {
    //load_configure(*this, path);
}

/* load graph from file with ".col" suffix. */
void load_graph_col(Graph &graph, const String &path) {
    ifstream ifs(path);
    if (!ifs.is_open()) {
        throw "Can't open instance file!";
    }
    string line;
    while (getline(ifs, line))
        if (line.size() && line[0] == 'p') break;
    Set<String> edge_set;
    char c; int node1, node2;
    while (ifs >> c >> node1 >> node2) {
        if (node1 == node2) continue;   // remove self in edge.
        int n1 = graph.node_id_map.add_id(node1);
        int n2 = graph.node_id_map.add_id(node2);
        int temp = n1; n1 = min(n1, n2); n2 = max(temp, n2);
        string edge_str = to_string(n1) + '\0' + to_string(n2);
        if (edge_set.find(edge_str) != edge_set.end()) continue;    // remove duplicated edge.
        edge_set.insert(edge_str);
        if (graph.neighbors.size() <= n2)
            graph.neighbors.resize(n2 + 1);
        graph.neighbors[n1].push_back(n2);
        graph.neighbors[n2].push_back(n1);
    }
    graph.nb_node = static_cast<int>(graph.neighbors.size());
    graph.nb_edge = static_cast<int>(edge_set.size());
    mylog << "load instance:" << path << logsw_info;
    mylog << "graph information: node number£º" << graph.nb_node
        << ";edge number:" << graph.nb_edge << logsw_info;
}

void load_configure(Configure &cfg, const String &path) {
    // [zjl][TODO] : add implement.
    throw "Not implement!";
}

void save_solution(const Solution &sol, const String &path, const IdMapInt<int> &id_map) {
    ofstream ofs(path);
    if (!ofs.is_open())
        return;
    unordered_map<int, List<int>> nodes_set;
    for (int n = 0; n < sol.nb_node(); ++n) {
        if (nodes_set.count(sol[n])) {
            nodes_set[sol[n]].push_back(n);
        } else {
            nodes_set[sol[n]] = { n };
        }
    }
    ofs << "n " << sol.nb_node() << endl;
    ofs << "c " << nodes_set.size() << endl;
    for (int c = 0; c < sol.nb_color(); ++c) {
        for (int node : nodes_set[c]) {
            ofs << id_map.get_id(node) << " ";
        }
        ofs << endl;
    }
}

Solution::Solution(String &path) {
    ifstream ifs(path);
    if (!ifs.is_open())return;
    string line;
    char c; int num;
    // first line: node number.
    getline(ifs, line);
    istringstream(line) >> c >> num;
    node_color_.resize(num, INVALID);
    // second line: color number.
    getline(ifs, line);
    istringstream(line) >> c >> num;
    color_size_.resize(num, 0);
    // create initial solution.
    int cur_color = 0;
    while (getline(ifs, line)) {
        istringstream istr(line);
        while (istr >> num) {
            node_color_[num] = cur_color;
            ++color_size_[cur_color];
        }
        ++cur_color;
    }
}

}
