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

    std::pair<std::array<Point, CONNECTIVITY>, int> movable_neighbors(Point const&) const noexcept;
    std::pair<std::array<int, CONNECTIVITY>, int> movable_neighbors(int global_cell_idx) const noexcept;

    std::pair<const ECellType*, int> row(int row_idx) const noexcept;
    int cells_num() const noexcept;

    /**
     * @brief generate width and height field with random wall cell placement
     * @param width
     * @param height
     * @param wall_num number of inital walls. If -1 number of wall cells is random
     * @return successful
     */
    bool generate(int width, int height, int wall_num) noexcept;

private:
    std::vector<ECellType> m_field{};
    int m_width{};

}; // class


#endif // FIELD_H
