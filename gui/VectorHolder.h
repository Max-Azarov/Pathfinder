#ifndef VECTORHOLDER_H
#define VECTORHOLDER_H

#include <vector>

template<class T>
struct VectorHolder {
    VectorHolder(std::vector<T>&& v)
        : value(std::move(v))
    {    }

    std::vector<T> value;
};

#endif // VECTORHOLDER_H
