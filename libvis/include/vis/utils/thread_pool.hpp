/**
 * @file utils/thread_pool.hpp
 * @brief Simple thread pool.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_THREAD_POOL_HPP
#define VIS_UTILS_THREAD_POOL_HPP

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

namespace vis {

/**
 * @brief Simple thread pool using Boost Asio and Boost Thread.
 */
class ThreadPool {
public:

    /// Creates a new pool of given size.
    ThreadPool(std::size_t);

    /// Destructor.
    ~ThreadPool();

    /// Submit a new task.
    template <typename Task>
    void submit(Task);

private:
    using asio_worker = std::unique_ptr<boost::asio::io_service::work>;

    boost::asio::io_service service;
    asio_worker working;
    std::vector<std::unique_ptr<boost::thread>> workers;
};

template <typename Task>
inline void
ThreadPool::submit(Task task) {
    service.post(task);
}

} // namespace vis

#endif // VIS_UTILS_THREAD_POOL_HPP

