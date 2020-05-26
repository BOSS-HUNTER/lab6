#include <picosha2/picosha2.h>

#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup.hpp>
#include <csignal>

#include "HashResolver.h"

void init()
{
    boost::log::register_simple_formatter_factory<
        boost::log::trivial::severity_level,
        char
    >("Severity");
    static const std::string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

    auto sinkFile = boost::log::add_file_log(
        boost::log::keywords::file_name = "logs/log_%N.log",
        boost::log::keywords::rotation_size = 128 * 1024 * 1024,
        boost::log::keywords::format = format
    );
    sinkFile->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::trace
    );          // Log file setup

    auto sinkConsole = boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = format
    );
    sinkConsole->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );      // Log console setup

    boost::log::add_common_attributes();

    srand(time(nullptr));
}

int main(int argc, char *argv[])
{
    init();
    size_t threadAmount = (argc >= 2)
                          ? boost::lexical_cast<size_t>(argv[1])
                          : std::thread::hardware_concurrency();

    BOOST_LOG_TRIVIAL(trace) << "Thread amount " << threadAmount;

    std::vector<std::thread> threads;
    threads.reserve(threadAmount);
    for (size_t i = 0; i < threadAmount; i++) {
        threads.emplace_back(HashResolver::resolve);
    }

    BOOST_LOG_TRIVIAL(trace) << "Added signal handler " << threadAmount;

    for (std::thread &thread : threads) {
        thread.join();
    }

    return 0;
}