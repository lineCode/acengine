#ifndef __COMMON_UTIL_NET_H__
#define __COMMON_UTIL_NET_H__

namespace ACE
{
    bool AceSetNonblocking(int sockfd);
    int AceSocket();
    int AceBind(int sockfd, const char* pszAddr, unsigned int unPort);
    int AceListen(int sockfd, int backlog = 5);
    int AceConnect(int fd, const char* pszAddr, unsigned int unPort);
	void AceSetBuffSize(int fd, int nRcvBuf, int nSndBuf);
	void AceGetBuffSize(int fd);
    int AceAsyncRead(int sockfd, void* buf, size_t bufsize, int nTimeout);

}// namespace ACE
#endif 
