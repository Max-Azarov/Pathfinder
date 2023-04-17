#include <limits>
#include <queue>

#include "core/include/utils.h"



bool utils::bfs_search(Graph const& graph, int start, int goal, std::vector<int>& path)
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

        for(auto const& v : graph.edges(u)) {

            if (colors[v] == EVertexColor::WHITE) {
                path[v] = u;
                if (v == goal) {
                    return true;
                }
                colors[v] = EVertexColor::GRAY;
                q.push(v);
            }
        }
        q.pop();
        colors[u] = EVertexColor::BLACK;
    }

    return false;
}
