#include <QDebug>

#include "core/include/Worker.h"
#include "core/include/utils.h"
#include "core/include/Graph.h"


#define NAME_VALUE(text) __FILE__ << ':' << __LINE__ << ':' << #text << ": " << text


std::vector<int> Worker::findPath(int start, int goal) noexcept
{
    auto graph = Graph(&m_field);

    auto path = std::vector<int>{};
    auto result = false;

    try {
        result = utils::bfsSearch(graph, start, goal, path);
    }
    catch(...) {
        // TODO
    }

    if (!result) {
//        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!result";
        path.clear();
    }

    return path;
}

bool Worker::generateField(int width, int height) noexcept
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
