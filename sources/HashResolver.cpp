#include "HashResolver.h"

namespace HashResolver
{

void resolve()
{
    static const std::string expectedEnd = "0000";
    const size_t expectedEndSize = expectedEnd.size();

    while (true) {
        std::string randomString = std::to_string(std::rand());
        std::string result = picosha2::hash256_hex_string(randomString);

        if (result.substr(result.size() - expectedEndSize) == expectedEnd) {
            BOOST_LOG_TRIVIAL(info) << "Found correct value '" << randomString << "' with hash '" << result << "'";
        } else {
            BOOST_LOG_TRIVIAL(trace) << "Current value '" << randomString << "' with hash '" << result << "'";
        }
    }
}

}