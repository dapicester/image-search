/**
 * @file utils/thread_pool.cpp
 * @brief Simple thread pool.
 * @author Paolo D'Apice
 */

#include "vis/utils/thread_pool.hpp"

namespace vis {

ThreadPool::ThreadPool(std::size_t threads)
       : service(), working(new asio_worker::element_type(service)) {
    // start workers
    for (size_t i = 0; i < threads; ++i) {
        workers.push_back(
            std::unique_ptr<boost::thread>(
                new boost::thread([this] {
                    service.run();
                })
            )
        );
    }
}

ThreadPool::~ThreadPool() {
#if 0
    service.stop();
    for ( std::size_t i = 0; i < workers.size(); ++i )
       workers[i]->join();
#else
    working.reset();
    service.run();
#endif
}

} // namespace vis

