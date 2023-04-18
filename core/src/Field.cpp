#include "core/include/Field.h"



std::pair<std::array<Field::Point, 4>, int> Field::movable_neighbors(Field::Point const& point) const noexcept
{
    auto const global_cell_idx = point.y * m_width + point.x;
    auto globals = movable_neighbors(global_cell_idx);

    auto result_array = std::array<Field::Point, CONNECTIVITY>{};
    auto it_result_array = result_array.begin();

    for (auto global_idx : globals.first) {
        *it_result_array++ = {global_idx % m_width, global_idx / m_width};
    }

    return {result_array, globals.second};
}

std::pair<std::array<int, 4>, int> Field::movable_neighbors(int global_cell_idx) const noexcept
{
    auto const offsets = std::array<int, CONNECTIVITY> {
        -1, 1, -m_width, m_width
    };

    auto const cell_num = cells_num();

    auto is_idx_out_of_bounds = [this, cell_num](int idx) -> bool {
        return  idx < 0 ||
                idx >= cell_num;
    };

    auto is_offset_out_of_bounds = [this, cell_num, &is_idx_out_of_bounds](int idx, int offset) -> bool {
        auto const dest_idx = idx + offset;
        return  is_idx_out_of_bounds(dest_idx) ||
                (offset == 1  && dest_idx % m_width == 0) ||
                (offset == -1 && (dest_idx + 1) % m_width == 0)
                ;
    };

    if (is_idx_out_of_bounds(global_cell_idx)) {
        return {};
    }

    auto result_size = 0;
    auto result_array = std::array<int, CONNECTIVITY>{};
    auto it_result_array = result_array.begin();

    for (auto offset : offsets) {
        auto idx = global_cell_idx + offset;
        if (!is_offset_out_of_bounds(global_cell_idx, offset) &&
            m_field[idx] != Field::ECellType::WALL) {
            result_size++;
            *it_result_array++ = idx;
        }
    }

    return {result_array, result_size};
}

std::pair<const Field::ECellType*, int> Field::row(int row_idx) const noexcept
{
    auto const idx = row_idx * m_width;
    if (idx < 0 || idx >= m_field.size()) {
        return {};
    }

    return {&m_field[idx], m_width};
}

int Field::cells_num() const noexcept
{
    return int(m_field.size());
}

bool Field::generate(int width, int height, int wall_num) noexcept
{
    if (width < 1 || height < 1) {
        return false;
    }

    try {
        m_field.assign(width * height, Field::ECellType::MOVABLE);
        m_field[0] = Field::ECellType::WALL;
        m_field[11] = Field::ECellType::WALL;
        m_field[12] = Field::ECellType::WALL;
        m_field[13] = Field::ECellType::WALL;
        m_field[14] = Field::ECellType::WALL;
        m_field[width * height / 2] = Field::ECellType::WALL;
        m_field[width * height - 1] = Field::ECellType::WALL;

        m_width = width;

        return true;
    }
    catch(...) {
        // TODO
    }

    return false;
}
