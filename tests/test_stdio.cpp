#include "logxx/logger_stdio.h"
#include <string>
#include <doctest/doctest.h>

DOCTEST_TEST_CASE("logger_stdio") {
    char buffer[4096] = { 0 };

    logxx::logger_stdio stdio(stdout);
    logxx::scoped_logger scoped(stdio);

    // ensure writing to stdout goes to our custom buffer, and won't be flushed
    // but a line-end character.
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

    LOXX_LOG_INFO("testing stdio");

    setvbuf(stdout, nullptr, _IOLBF, BUFSIZ);

    DOCTEST_CHECK(std::string(buffer).find("testing stdio") != std::string::npos);
}
