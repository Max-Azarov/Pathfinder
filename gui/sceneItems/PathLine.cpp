#include <QPen>
#include <QGraphicsLineItem>

#include "gui/sceneItems/PathLine.h"
#include "gui/control/Manager.h"


// --------------------------------------------------------------------------------------------------------------------
PathLine::PathLine(Manager* manager)
    : m_manager(manager)
{

}
// --------------------------------------------------------------------------------------------------------------------
bool PathLine::reserve(int n) noexcept
{
    try {
        m_items.reserve(n);
        return true;
    }
    catch(...){}

    return false;
}
// --------------------------------------------------------------------------------------------------------------------
bool PathLine::add(QLineF const& line) noexcept
{
    try {
        auto pLine = std::make_unique<QGraphicsLineItem>(line);
        pLine->setPen(QPen(PathLine::lineColor));
        m_items.emplace_back(std::move(pLine));
        return true;
    }
    catch(...){}

    return false;
}
// --------------------------------------------------------------------------------------------------------------------
