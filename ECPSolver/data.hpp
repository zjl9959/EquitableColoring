#ifndef ECPSOLVER_DATA_H
#define ECPSOLVER_DATA_H

#include "common.hpp"
#include "utils/utility.h"

namespace solver {

struct Environment {
    #pragma region constructor
    Environment(const String &_instance_name, const String &_configure_name = "") :
        instance_name(_instance_name), configure_name(_configure_name) {};
    #pragma endregion

    #pragma region interface
    const String instance_path() { return instance_dir() + instance_name; }
    const String solution_path() { return solution_dir() + instance_name + ".txt"; }
    const String solution_path_withtime() { return solution_dir() + instance_name + zjl_utility::Date::shortDateStr() + ".txt"; }
    const String configure_path() { return configure_dir() + configure_name; }
    
    static const String instance_dir() { return "Instances/"; }
    static const String solution_dir() { return "Solutions/"; }
    static const String configure_dir() { return "Configures/"; }
    #pragma endregion

    #pragma region variables
    const String instance_name;
    const String configure_name;
    #pragma endregion
};

struct Configure {
    #pragma region constructor
    Configure() {};
    Configure(const String &path);
    #pragma endregion

    #pragma region variables
    // [zjl][TODO] : add variables.
    #pragma endregion
};

struct Assignment {
    int node;   // node identity.
    int color;  // the color assigned to node.
    Assignment(int _node = INVALID, int _color = INVALID) : node(_node), color(_color) {};
};

struct Graph {
    #pragma region constructor
    Graph(const String &path);
    #pragma endregion

    #pragma region variables
    size_t nb_node;
    size_t nb_edge;
    List<List<int>> neighbors;
    zjl_utility::IdMapInt<int> node_id_map;
    #pragma endregion
};

struct Input {
    #pragma region constructor
    Input(int _nb_color, long long _timeout_ms, unsigned int _rand_seed,
        const String &_instance_name, const String &_configure_name = "") :
        nb_color(_nb_color), timeout_ms(_timeout_ms), rand(_rand_seed),
        env(_instance_name, _configure_name), cfg(env.configure_path()), graph(env.instance_path()) {};
    #pragma endregion

    #pragma region variables
    int nb_color;   // available color number.
    long long timeout_ms;   // maximum time out millisecond.
    zjl_utility::Random rand;

    Environment env;
    Configure cfg;
    Graph graph;
    #pragma endregion
};

struct Output {
    #pragma region interface
    void save(const String &path, const zjl_utility::IdMapInt<int> &id_map);
    #pragma endregion

    #pragma region variables
    List<Assignment> assignments;
    #pragma endregion
};

}

#endif // !ECPSOLVER_DATA_H
