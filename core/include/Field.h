#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <array>


template<int Connectivity>
class Field {
public:
    enum class ECellType {
        WALL, MOVABLE
    };

    struct Point {
        int x, y;
    };

    std::pair<std::array<Point, Connectivity>, int> movable_neighbors(Point const&) const;
    std::pair<std::array<int, Connectivity>, int> movable_neighbors(int global_cell_num) const;

    std::pair<const ECellType*, int> row(int idx) const;
    int cell_num() const;

    /**
     * @brief generate width and height field with random wall cell placement
     * @param width
     * @param height
     * @param wall_num number of inital walls. If -1 number of wall cells is random
     * @return successful
     */
    bool generate(int width, int height, int wall_num);

private:
    std::vector<ECellType> m_field{};
    int m_width{};

}; // class


#endif // FIELD_H
