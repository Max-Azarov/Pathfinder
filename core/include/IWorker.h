#ifndef WORKER_H
#define WORKER_H

#include <memory>


struct IWorker {




    static std::unique_ptr<IWorker> create() noexcept;

}; // class

#endif // WORKER_H
