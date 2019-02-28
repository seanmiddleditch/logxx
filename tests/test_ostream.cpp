#include "logxx/logger_ostream.h"
#include <sstream>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("logger_ostream") {
    std::ostringstream str;
    logxx::logger_ostream stream(str);
    logxx::scoped_logger scoped(stream);

    LOXX_LOG_INFO("testing ostream");

    DOCTEST_CHECK(str.str().find("testing ostream") != std::string::npos);
}
