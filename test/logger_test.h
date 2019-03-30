#ifndef __LOGGER_TEST_H__
#define __LOGGER_TEST_H__
#include "../common/logger.h"
void test_of_logger() {
	ACE::g_logger.SetLogPath("./logs/testapp/");
	ACE::LogInfo("testinfo", "sss");
	ACE::LogDebug("debug", "debug info");
}

#endif
