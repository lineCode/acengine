#include <iostream>
#include <string>
#include "../common/mutex.h"
#include "../common/logger.h"


using namespace std;

int main()
{
	cout<<"in gatesrv"<<endl;

	ACE::g_logger_mutex = new pthread_mutex_t; 
	return 0;
}
