#include <limits>
#include <queue>

#include "core/include/utils.h"


bool utils::bfsSearch(Graph const& graph, int start, int goal, std::vector<int>& path)
{
    auto const n = graph.numVertices();
    path.assign(n, -1);

    enum class EVertexColor {
        WHITE, BLACK, GRAY
    };

    auto colors = std::vector<EVertexColor>(n, EVertexColor::WHITE);

    std::queue<int> q;
    q.push(start);

    while (!q.empty()) {
        auto const u = q.front();

        auto const vertices = graph.connectedVertices(u);
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
