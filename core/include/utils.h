#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <queue>

#include "core/include/Graph.h"


namespace utils {

template<class Graph>
bool bfs_search(Graph const& graph, int start, int goal, std::vector<int>& path)
{
    auto const n = graph.num_vertices();
    path.assign(n, -1);

    enum class EVertexColor {
        WHITE, BLACK, GRAY
    };

    std::vector<EVertexColor> colors (n, EVertexColor::WHITE);

    std::queue<int> q;
    q.push(start);

    while (!q.empty()) {
        auto const u = q.front();

        auto const vertices = graph.connected_vertices(u);
        for (auto v = vertices.first.cbegin(), end = vertices.first.cbegin() + vertices.second; v != end; v++) {

            if (colors[*v] == EVertexColor::WHITE) {
                path[*v] = u;
                if (*v == goal) {
                    return true;
                }
                colors[*v] = EVertexColor::GRAY;
                q.push(*v);
            }
        }
        q.pop();
        colors[u] = EVertexColor::BLACK;
    }

    return false;
}

} // namespace


#endif // UTILS_H
