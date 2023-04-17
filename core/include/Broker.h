#ifndef BROKER_H
#define BROKER_H

#include <memory>

class Broker {
public:

    enum EType{
        BFS,
    };

    template<EType Type>
    std::unique_ptr<Broker> create();
};

#endif // BROKER_H
