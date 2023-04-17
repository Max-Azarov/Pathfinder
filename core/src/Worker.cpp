#include "core/include/IWorker.h"
#include "core/include/Field.h"
#include "core/include/utils.h"


class Worker : public IWorker
{
private:
    Field<4> m_field{};


public:
    Worker() {}

}; // class


std::unique_ptr<IWorker> IWorker::create() noexcept {
    try {
        return std::make_unique<Worker>();
    }
    catch(...) {
        return {};
    }
}
