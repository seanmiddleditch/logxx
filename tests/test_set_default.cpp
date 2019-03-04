#include "logxx/logger_ostream.h"
#include <sstream>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("set_default_logger") {
    std::ostringstream str;
    logxx::logger_ostream stream(str);
    logxx::logger_base* old_default = nullptr;

    logxx::set_default_logger(&stream, &old_default);
    LOGXX_LOG_INFO("testing ostream");
    logxx::set_default_logger(old_default);

    DOCTEST_CHECK(str.str().find("testing ostream") != std::string::npos);
}
