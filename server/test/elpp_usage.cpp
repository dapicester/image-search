/**
 * @file elpp_usage.cpp
 * @brief Usage tests for the EasyLogging++ library.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE easylogging
#include <boost/test/unit_test.hpp>

#define _ELPP_THREAD_SAFE
#define _ELPP_NO_DEFAULT_LOG_FILE
//#define _ELPP_DEFAULT_LOG_FILE "test_logs.txt"

#include <easylogging++.h>
#include <thread>

// must be one once
_INITIALIZE_EASYLOGGINGPP

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(elpp_log) {
    LOG(TRACE)   << "Trace log";
    LOG(DEBUG)   << "Debug log";
    LOG(INFO)    << "Info log";
    LOG(WARNING) << "Warning log";
    LOG(ERROR)   << "Error log";
}

struct ElppInitializer {
    ElppInitializer() {
        el::Configurations testConf;
        testConf.setToDefault();
        testConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime %level (%thread) %msg");
        el::Loggers::reconfigureLogger("test", testConf);

        el::Configurations anotherConf;
        anotherConf.setToDefault();
        el::Loggers::reconfigureLogger("another", anotherConf);
    }
};

BOOST_FIXTURE_TEST_CASE(elpp_clog, ElppInitializer) {
    CLOG(TRACE,"test")   << "Trace log";
    CLOG(DEBUG,"test")   << "Debug log";
    CLOG(INFO,"test")    << "Info log";
    CLOG(WARNING,"test") << "Warning log";
    CLOG(ERROR,"test")   << "Error log";

    CLOG(INFO, "another") << "Another log";
}

BOOST_AUTO_TEST_CASE(elpp_threads) {
    // TODO open issue
    el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%tid", []() -> const char* {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str().c_str();
    }));
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level (%thread) [%tid] %msg");

    std::thread([]{
        LOG(INFO) << "log from thread";
    }).join();

    LOG(INFO) << "outside thread";
}

/// Run test with \c --v=N or \c -v|--verbose
BOOST_AUTO_TEST_CASE(elpp_vlog) {
    // must initialize with argc/argv
    _START_EASYLOGGINGPP(argc, argv);

    VLOG(1) << "Verbose 1 log";
    VLOG(2) << "Verbose 2 log";
    VLOG(3) << "Verbose 3 log";
    VLOG(4) << "Verbose 4 log";
    VLOG(5) << "Verbose 5 log";
    VLOG(6) << "Verbose 6 log";
    VLOG(7) << "Verbose 7 log";
    VLOG(8) << "Verbose 8 log";
    VLOG(9) << "Verbose 9 log";

    if (VLOG_IS_ON(2)) {
        LOG(DEBUG) << "Only on verbose=2";
    }
}

// Third-party class
struct MyClass {
    int i;
    std::string s;
};

inline MAKE_LOGGABLE(MyClass, c, os) {
    return os << c.s << ":" << c.i;
}

BOOST_AUTO_TEST_CASE(elpp_loggable) {
    MyClass c = { 1, "hello" };
    LOG(INFO) << "class: " << c;
}

