#include <QDebug>

#include "core/include/Worker.h"
#include "core/include/utils.h"
#include "core/include/Graph.h"


#define NAME_VALUE(text) __FILE__ << ':' << __LINE__ << ':' << #text << ": " << text


bool Worker::findPath(int start, int goal)
{
    auto const width = 5;
    auto const height = 10;
    m_field.generate(width, height, -1);
    auto graph = Graph(&m_field);

    auto path = std::vector<int>{};
    auto result = false;

    try {
        result = utils::bfsSearch(graph, start, goal, path);
    }
    catch(...) {
        // TODO
    }

    qDebug() << NAME_VALUE(result);

    return result;
}

bool Worker::generateField(int width, int height)
{
    return m_field.generate(width, height, -1);
}


std::unique_ptr<Worker> Worker::create() noexcept {
    try {
        return std::make_unique<Worker>();
    }
    catch(...) {
        return {};
    }
}
