#include "logxx/logxx.h"
#include "logxx/logger_ostream.h"
#include <sstream>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("section") {
    logxx::section sec("test", logxx::log_level::info);

    std::ostringstream str;
    logxx::logger_ostream stream(str);
    logxx::scoped_logger scoped(stream);

    LOGXX_LOG_TO(sec, logxx::log_level::debug, "testing disabled");

    DOCTEST_CHECK(str.str().empty());

    LOGXX_LOG_TO(sec, logxx::log_level::info, "testing enabled");

    DOCTEST_CHECK(str.str().find("testing enabled") != std::string::npos);
}
