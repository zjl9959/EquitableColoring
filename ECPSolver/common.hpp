#ifndef ECPSOLVER_COMMON_H
#define ECPSOLVER_COMMON_H

#include <vector>
#include <string>

namespace solver {

template<typename T>
using List = std::vector<T>;

using String = std::string;

constexpr int INVALID = -1;

}

#endif // !ECPSOLVER_COMMON_H
