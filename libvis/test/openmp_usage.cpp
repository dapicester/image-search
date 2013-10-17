/**
 * @file openmp_usage.cpp
 * @brief OpenMP usage test.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE openmp
#include <boost/test/unit_test.hpp>

#ifndef _OPENMP
#  error "OpenMP is required to build this test!"
#endif

#include <omp.h>
#include <cmath>
#include <cstdio>

#define pr(X) printf(#X ": %d\n",(X))

BOOST_AUTO_TEST_CASE(openmp_functions) {
    BOOST_CHECK_EQUAL(omp_get_num_threads(), 1);    // called ouside omp block

    #pragma omp parallel
    BOOST_CHECK_GE(omp_get_num_threads(), 1);       // called inside omp block

    BOOST_CHECK_GE(omp_get_num_procs(), 1);
    BOOST_CHECK_GE(omp_get_max_threads(), 1);
}

/// Executes code on multiple threads in a \b new team.
BOOST_AUTO_TEST_CASE(openmp_parallel) {
    #pragma omp parallel
    printf("omp parallel: thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
}

/// Explicit number of threads.
BOOST_AUTO_TEST_CASE(openmp_num_threads) {
    #pragma omp parallel num_threads(2)
    printf("omp parallel: thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
}

/// Share work among threads in the \b current team (actually only 1 thread).
BOOST_AUTO_TEST_CASE(openmp_for) {
    #pragma omp for
    for (int i = 0; i < 10; i++)
        printf("%d ", i);

    printf("\n");
}

/// Share work among threads in the created team.
BOOST_AUTO_TEST_CASE(openmp_for_parallel) {
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < 10; i++)
            printf("%d ", i);
    }

    printf("\n");
}

/// Same as above but in compact form.
BOOST_AUTO_TEST_CASE(openmp_parallel_for) {
    #pragma omp parallel for num_threads(3)
    for (int i = 0; i < 10; i++)
        printf("%d ", i);

    printf("\n");
}

/// Use private when using C style declarations.
BOOST_AUTO_TEST_CASE(openmp_private) {
    int i;
    #pragma omp parallel for private(i)
    for (i = 0; i < 10; i++)
        printf("%d ", i);

    printf("\n");
}

/// Using private and shared variables.
/// Single processor opcodes can be defined \c atomic.
BOOST_AUTO_TEST_CASE(openmp_shared) {
    int i;
    int sum = 0; // Always initialize shared variables.
    #pragma omp parallel for private(i) shared(sum)
    for (i = 0; i < 10; i++) {
        #pragma omp atomic
        sum += i;
    }
    BOOST_CHECK_EQUAL(45, sum);
}

/// Same as befor but with \c reduction clause.
BOOST_AUTO_TEST_CASE(openmp_reduction) {
    int sum = 0; // Always initialize shared variables.
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 10; i++) {
        sum += i;
    }
    BOOST_CHECK_EQUAL(45, sum);
}

/// If previous value of private variables is needed, use \c firstprivate.
/// Use \c critical sections for operations.
BOOST_AUTO_TEST_CASE(openmp_firstprivate) {
    {
        std::string a = "x", b = "y";
        int c = 3;
        #pragma omp parallel private(a,c) shared(b) num_threads(2)
        {
            a += "k";
            c += 7;
            #pragma omp critical
            std::cout << "A becomes (" << a << "), b is (" << b << ")\n";
        }
    }
    {
        std::string a = "x", b = "y";
        int c = 3;
        #pragma omp parallel firstprivate(a,c) shared(b) num_threads(2)
        {
            a += "k";
            c += 7;
            #pragma omp critical
            std::cout << "A becomes (" << a << "), b is (" << b << ")\n";
        }
    }
}

/// Specify ordering.
/// There must be <b>exactly one</b> \c ordered block per \c ordered loop.
BOOST_AUTO_TEST_CASE(openmp_ordered) {
    #pragma omp parallel for ordered
    for(int n=0; n<10; ++n) {
        #pragma omp ordered
        printf("%d ", n);
    }
    printf("\n");
}

/// Let a block to be executed by only one thread.
BOOST_AUTO_TEST_CASE(openmp_single) {
    #pragma omp parallel num_threads(2)
    {
        printf("shared work1\n");

        #pragma omp single
        printf("single work\n");

        printf("shared work2\n");
    }
}

/// Let all thread wait each other for completion.
BOOST_AUTO_TEST_CASE(openmp_barrier) {
    #pragma omp parallel num_threads(4)
    {
        printf("shared work1\n");

        #pragma omp barrier

        printf("shared work2\n");
    }
}

/// Parallel sections, each section block runs in parallel.
BOOST_AUTO_TEST_CASE(openmp_sections) {
    #pragma omp parallel sections
    {
        { printf("Section 1\n"); }

        #pragma omp section
        { printf("Section 2,1\n");
          printf("Section 2,2\n"); }

        #pragma omp section
        { printf("Section 3\n"); }
    }
}

/// Use also with iterators (requires OpenMP 3.0).
BOOST_AUTO_TEST_CASE(openmp_iterators) {
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<int> vec(data, data+10);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::vector<int> cp;
    int i = 0, sum = 0;

    #pragma omp parallel for default(none) shared(i,sum,vec,cp)
    for (std::vector<int>::iterator it = vec.begin(); it < vec.end(); it++) {
        printf("#%d:\t*it=%d\ti=%d\n", omp_get_thread_num(), *it, i);
        #pragma omp critical
        {
            cp.push_back(*it);
            i++;
            sum += *it;
        }
    }

    BOOST_CHECK_EQUAL(10, i);
    BOOST_CHECK_EQUAL(10, cp.size());
    BOOST_CHECK_EQUAL(55, sum);

    std::copy(cp.begin(), cp.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

