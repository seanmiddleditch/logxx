#include "logxx/logger_stdio.h"

void test_stdio() {
    logxx::logger_stdio stdio(stdout);
    logxx::scoped_logger scoped(stdio);

    LOXX_LOG_INFO("testing stdio");
}
