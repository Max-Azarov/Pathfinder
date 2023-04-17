#include "core/include/Broker.h"


class BrokerBfs : public Broker
{
public:
    BrokerBfs() {}
}; // class

template<>
std::unique_ptr<Broker> Broker::create<Broker::BFS>() {
    try {
        return std::make_unique<BrokerBfs>();
    }
    catch(...) {
        return {};
    }
}
