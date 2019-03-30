#ifndef __COMM_WORLD__
#define __COMM_WORLD__
#include "cfg_reader.h"

namespace ACE
{

class world
{
public:
	world();
	virtual ~world();
public:
	virtual int init(const char *cfgfile);	
protected:
	CCfgReader *_cfg;
};

}
#endif 
