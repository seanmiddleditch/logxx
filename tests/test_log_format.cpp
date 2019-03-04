#include "logxx/log_format.h"
#include "logxx/logger_ostream.h"
#include <sstream>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("log_format") {
    std::ostringstream str;
    logxx::logger_ostream stream(str);
    logxx::scoped_logger scoped(stream);

    LOGXX_LOG_FMT(logxx::log_level::info, "number {} bool {} string {}", 1, true, "abc");

    DOCTEST_CHECK(str.str().find("number 1 bool true string abc") != std::string::npos);
}
