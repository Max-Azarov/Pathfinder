#include <random>
#include <chrono>

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

    m_width = width;
    auto const cell_num = width * height;

    try {
        m_field.assign(cell_num, Field::ECellType::MOVABLE);

        // random indices for wall cells
        std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

        if (wall_num < 0) {
            wall_num = std::uniform_int_distribution<int>(cell_num / 8, cell_num / 4)(gen);
        }
        wall_num = std::min(cell_num, wall_num);

        if (wall_num) {
            auto seq = std::vector<int>(m_field.size());
            std::iota(seq.begin(), seq.end(), 0);

            for (int i = 0; i < wall_num; i++) {
                auto const idx = std::uniform_int_distribution<int>(i, cell_num - 1)(gen);
                m_field[seq[idx]] = Field::ECellType::WALL;
                std::swap(seq[i], seq[idx]);
            }
        }

        return true;
    }
    catch(...) {
        // TODO
    }

    return false;
}
