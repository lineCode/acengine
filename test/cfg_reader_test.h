#ifndef __TEST_CFG_READER_TEST_H__
#define __TEST_CFG_READER_TEST_H__
#include "../common/cfg_reader.h"
#include <cassert>
void test_cfg_reader() {
	ACE::CCfgReader cfg("./cfg.ini");
	cfg.show();
	assert(cfg.GetValue("server_1", "ip")=="127.0.0.1");
	assert(cfg.GetValue("server_2", "port")=="8889");
}

#endif
