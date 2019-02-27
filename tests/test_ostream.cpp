#include "logxx/logger_ostream.h"
#include <iostream>

void test_ostream() {
    logxx::logger_ostream cout(std::cout);
    logxx::scoped_logger scoped(cout);

    LOXX_LOG_INFO("testing ostream");
}
