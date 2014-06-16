/**
 * @file test_thread_pool.cpp
 * @brief Usage test for the thread pool.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE thread_pool
#include <boost/test/unit_test.hpp>

#include "vis/utils/thread_pool.hpp"

BOOST_AUTO_TEST_CASE(pool_test) {
    const std::size_t POOL_SIZE = boost::thread::hardware_concurrency();
    const std::size_t NUM_TASKS = 8;

    vis::ThreadPool pool(POOL_SIZE);

    for (int i = 0; i < NUM_TASKS; ++i) {
        std::cout << "task " << i << " created" << std::endl;
        pool.submit([i] {
            std::cout << "hello " << i << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(300));
            std::cout << "world " << i << std::endl;
        });
    }
}

