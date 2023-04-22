#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <array>


class Field {
public:
    static auto constexpr CONNECTIVITY = 4;

    enum class ECellType {
        WALL, MOVABLE
    };

    struct Point {
        int x, y;
    };

    std::pair<std::array<Point, CONNECTIVITY>, int> movableNeighbors(Point const&) const noexcept;
    std::pair<std::array<int, CONNECTIVITY>, int> movableNeighbors(int globalCellIdx) const noexcept;

    std::pair<const ECellType*, int> row(int rowIdx) const noexcept;
    int cellsNum() const noexcept;

    /**
     * @brief generate width and height field with random wall cell placement
     * @param width
     * @param height
     * @param wallNum number of inital walls. If <0 number of wall cells is random
     * @return successful
     */
    bool generate(int width, int height, int wallNum) noexcept;

private:
    std::vector<ECellType> m_field{};
    int m_width{};

}; // class


#endif // FIELD_H
