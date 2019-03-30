#include "world.h"
namespace ACE
{

world::world() {

}

world::~world() {
	delete _cfg;
}

int world::init(const char *cfgfile) {
	_cfg = new CCfgReader(cfgfile);
	return 0;
}


}// namespace ACE

