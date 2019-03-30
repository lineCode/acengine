#include <iostream>
#include <string>
#include <cassert>
#include "cfg_reader_test.h"
#include "logger_test.h"

using namespace std;

#define CFG_READER
int main()
{
#ifdef CFG_READER
	//test_cfg_reader();
#endif 
	//test_of_logger();

	cout<<"in test"<<endl;
	return 0;
}
