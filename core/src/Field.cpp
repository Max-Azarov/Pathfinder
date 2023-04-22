#include <random>
#include <chrono>

#include "core/include/Field.h"



std::pair<std::array<Field::Point, 4>, int> Field::movableNeighbors(Field::Point const& point) const noexcept
{
    auto const globalCellIdx = point.y * m_width + point.x;
    auto globals = movableNeighbors(globalCellIdx);

    auto result_array = std::array<Field::Point, CONNECTIVITY>{};
    auto it_result_array = result_array.begin();

    for (auto global_idx : globals.first) {
        *it_result_array++ = {global_idx % m_width, global_idx / m_width};
    }

    return {result_array, globals.second};
}

std::pair<std::array<int, 4>, int> Field::movableNeighbors(int globalCellIdx) const noexcept
{
    auto const offsets = std::array<int, CONNECTIVITY> {
        -1, 1, -m_width, m_width
    };

    auto const cell_num = cellsNum();

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

    if (is_idx_out_of_bounds(globalCellIdx)) {
        return {};
    }

    auto result_size = 0;
    auto result_array = std::array<int, CONNECTIVITY>{};
    auto it_result_array = result_array.begin();

    for (auto offset : offsets) {
        auto idx = globalCellIdx + offset;
        if (!is_offset_out_of_bounds(globalCellIdx, offset) &&
            m_field[idx] != Field::ECellType::WALL) {
            result_size++;
            *it_result_array++ = idx;
        }
    }

    return {result_array, result_size};
}

std::pair<const Field::ECellType*, int> Field::row(int rowIdx) const noexcept
{
    auto const idx = rowIdx * m_width;
    if (idx < 0 || idx >= m_field.size()) {
        return {};
    }

    return {&m_field[idx], m_width};
}

int Field::cellsNum() const noexcept
{
    return int(m_field.size());
}

bool Field::generate(int width, int height, int wallNum) noexcept
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

        if (wallNum < 0) {
            wallNum = std::uniform_int_distribution<int>(cell_num / 8, cell_num / 4)(gen);
        }
        wallNum = std::min(cell_num, wallNum);

        if (wallNum) {
            auto seq = std::vector<int>(m_field.size());
            std::iota(seq.begin(), seq.end(), 0);

            for (int i = 0; i < wallNum; i++) {
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
