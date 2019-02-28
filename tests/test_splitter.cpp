#include "logxx/logger_splitter.h"
#include "logxx/logger_ostream.h"
#include <sstream>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("logger_splitter") {
    std::ostringstream first, second;
    logxx::logger_ostream first_stream(first), second_stream(second);
    logxx::logger_splitter splitter(first_stream, second_stream);
    logxx::scoped_logger scoped(splitter);

    LOXX_LOG_INFO("testing splitter");

    DOCTEST_CHECK(first.str().find("testing splitter") != std::string::npos);
    DOCTEST_CHECK(second.str().find("testing splitter") != std::string::npos);
}
