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

/*
* load graph from file with ".col" suffix.
*/
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
    graph.nb_node = graph.neighbors.size();
    graph.nb_edge = edge_set.size();
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
    unordered_map<int, List<int>> color_nodes;
    for (int i = 0; i < sol.node_color.size(); ++i) {
        auto pos = color_nodes.find(sol.node_color[i]);
        if (pos != color_nodes.end()) {
            pos->second.push_back(i);
        } else {
            List<int> temp; temp.push_back(i);
            color_nodes.insert(make_pair(sol.node_color[i], temp));
        }
    }
    ofs << "nodes sets:" << endl;
    for (auto it = color_nodes.begin(); it != color_nodes.end(); ++it) {
        for (int node : it->second) {
            ofs << id_map.get_id(node) << " ";
        }
        ofs << endl;
    }
}

}
