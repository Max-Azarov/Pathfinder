#ifndef WORKER_H
#define WORKER_H

#include <memory>
#include <vector>


#include "core/include/Field.h"


class Worker : public QObject {

private:
    Field m_field{};

public:

    std::vector<int> findPath(int start, int goal)  noexcept;
    bool generateField(int width, int height);
    Field const& field() const { return m_field; }

    static std::unique_ptr<Worker> create() noexcept;

}; // class

#endif // WORKER_H
