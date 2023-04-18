#include <QDebug>

#include "core/include/IWorker.h"
#include "core/include/Field.h"
#include "core/include/utils.h"
#include "core/include/Graph.h"


#define NAME_VALUE(text) __FILE__ << ':' << __LINE__ << ':' << #text << ": " << text


class Worker : public IWorker
{
private:
    Field m_field{};


public:
    bool find_path(int start, int goal) override
    {
        auto const width = 5;
        auto const height = 10;
        m_field.generate(width, height, 3);
        auto graph = Graph(&m_field);

        auto path = std::vector<int>{};
        auto result = false;

        try {
            result = utils::bfs_search(graph, start, goal, path);
        }
        catch(...) {
            // TODO
        }

        qDebug() << NAME_VALUE(result);

        return result;
    }

}; // class


std::unique_ptr<IWorker> IWorker::create() noexcept {
    try {
        return std::make_unique<Worker>();
    }
    catch(...) {
        return {};
    }
}
