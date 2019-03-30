#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "util_net.h"

namespace ACE
{
    bool AceSetNonblocking(int sockfd) {
        return fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) != -1;
    }

    int AceSocket() {
        return socket(PF_INET, SOCK_STREAM, 0);
    }

    int AceBind(int sockfd, const char * pszaddr, unsigned int unport) {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_port = htons(unport);

        if (NULL == pszaddr || 0 == strcmp(pszaddr, "")) {
            addr.sin_addr.s_addr = INADDR_ANY;
        }
        else {
            addr.sin_addr.s_addr = inet_addr(pszaddr);
        }

        int flag = 1;
        int len = sizeof(int);
        // 关闭地址复用功能
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, len);
        return bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    }

    int AceListen(int sockfd, int backlog) {
        return listen(sockfd, backlog);
    }

    int AceConnect(int fd, const char *pszaddr, unsigned int unport) {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_port = htons(unport);
        addr.sin_addr.s_addr = inet_addr(pszaddr);

        return connect(fd, (sockaddr*)&addr, sizeof(addr));
    }

    void AceSetBuffSize(int fd, int n_recvbuf, int n_sendbuf) {
        setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const int*)&n_recvbuf, sizeof(int));
        setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const int*)&n_sendbuf, sizeof(int));
    }

}// namespace ACE