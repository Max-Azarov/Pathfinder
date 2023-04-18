#ifndef WORKER_H
#define WORKER_H

#include <memory>


struct IWorker {

    virtual bool find_path(int start, int goal) = 0;


    static std::unique_ptr<IWorker> create() noexcept;

}; // class

#endif // WORKER_H
