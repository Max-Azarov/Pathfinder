#ifndef GRAPH_H
#define GRAPH_H

#include <memory>

#include "core/include/Field.h"


class Graph {
public:
    explicit Graph(Field* field)
        : m_field(field)
    {}

    std::pair<std::array<int, 4>, int> connectedVertices(int u) const noexcept
    {
        if (!m_field) {
            return {};
        }

        return m_field->movableNeighbors(u);
    }


    int numVertices() const noexcept
    {
        if (!m_field) {
            return {};
        }

        return m_field->cellsNum();
    }


private:
    Field* m_field;

}; // class

#endif // GRAPH_H
