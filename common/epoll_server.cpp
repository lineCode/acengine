#include "epoll_server.h"
#include "util_net.h"

namespace ACE
{
	CEpollServer::CEpollServer():_epfd(0), the_world(NULL), _bshutdown(false) {

	
	}

	CEpollServer::~CEpollServer() {
	
	}

	int CEpollServer::StartServer(const char *addr, uint16_t port) {
		_straddr.assign(addr);
		_unport = port;
		int fd = AceSocket();

		return fd;
	}
	// 连接所有的服务器
	int CEpollServer::ConnectMailboxs(const char *cfgfile) {
		return 0;
	}

	int CEpollServer::Service(const char *szaddr, uint16_t port) {
		return 0;
	}

}
