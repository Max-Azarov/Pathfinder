#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <queue>

#include "core/include/Graph.h"


namespace utils {

bool bfs_search(Graph const& graph, int start, int goal, std::vector<int>& path);


} // namespace


#endif // UTILS_H
