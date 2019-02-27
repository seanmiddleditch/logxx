#include "logxx/logger_outputdebugstring.h"
#include <iostream>

void test_outputdebugstring() {
    logxx::logger_outputdebugstring ods;
    logxx::scoped_logger scoped(ods);

    LOXX_LOG_INFO("testing OutputDebugString");
}
