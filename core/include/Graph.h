#ifndef GRAPH_H
#define GRAPH_H

#include <memory>

#include "core/include/Field.h"


template<int MaxNumConnectedVertices>
class Graph {
public:
    using field_t = Field<MaxNumConnectedVertices>;

    std::pair<std::array<int, MaxNumConnectedVertices>, int> connected_vertices(int u) const noexcept
    {
        if (!m_field) {
            return {};
        }

        return m_field->movable_neighbors(u);
    }


    int num_vertices() const noexcept
    {
        if (!m_field) {
            return {};
        }

        return m_field->cell_num();
    }


    bool associate_graph_with_field(field_t* field) noexcept
    {
        if (!field) {
            return false;
        }

        m_field = field;
    }

private:
    field_t* m_field;

}; // class

#endif // GRAPH_H
