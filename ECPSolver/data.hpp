#ifndef ECPSOLVER_DATA_H
#define ECPSOLVER_DATA_H

#include "common.hpp"

namespace solver {

struct Environment {

};

struct Configure {

};

struct Assignment {
    int node;   // node identity.
    int color;  // the color assigned to node.
    Assignment(int _node = INVALID, int _color = INVALID) : node(_node), color(_color) {};
};

struct Edge {
    int st; // edge start node.
    int ed; // edge end node.
    Edge(int _st = INVALID, int _ed = INVALID) : st(_st), ed(_ed) {};
};

struct Graph {

};

struct Input {
    int nb_color;   // available color number.
    Environment env;
    Configure cfg;
    Graph graph;
};

struct Output {
    void save(String path);
};

}

#endif // !ECPSOLVER_DATA_H