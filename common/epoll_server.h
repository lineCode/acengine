#ifndef __COMMON_EPOLL_SERVER__
#define __COMMON_EPOLL_SERVER__
#include <map>
#include "world.h"
namespace ACE
{

class CEpollServer
{
public:
	CEpollServer();
	virtual ~CEpollServer();
public:
	int StartServer(const char *addr, uint16_t port);
	// 连接所有的服务器
	int ConnectMailboxs(const char *cfgfile);
public:
	int Service(const char *szaddr, uint16_t port);

protected:
	int _epfd;
	// 所有的连接
	//map<int, CMailBox*> _fds;
	// 
	world *the_world;
	bool _bshutdown;
	
	uint16_t _unport;
	string _straddr;
};

}// namespace ACE
#endif 
