#ifndef PATHLINE_H
#define PATHLINE_H

#include <vector>
#include <memory>

#include <QGraphicsLineItem>

class Manager;

class PathLine {
public:
    PathLine(Manager* manager);
    ~PathLine() = default;

    bool reserve(int) noexcept;
    bool add(QLineF const&) noexcept;

    std::vector<std::unique_ptr<QGraphicsLineItem>> const&
    items() noexcept
    { return m_items; }

private:
    Manager* m_manager = nullptr;
    std::vector<std::unique_ptr<QGraphicsLineItem>> m_items{};
    static constexpr Qt::GlobalColor lineColor = Qt::blue;
};

#endif // PATHLINE_H
