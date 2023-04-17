#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
public:

    std::vector<int> const& edges(int u) const
    {
        return v[u];
    }

    int num_vertices() const {}

private:
    std::vector<std::vector<int>> v;
    int m_vertex_num;
};

#endif // GRAPH_H
