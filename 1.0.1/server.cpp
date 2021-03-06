#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAXLINE 4096

int main(int argc, char** argv) {
	int listenfd, connfd;	// socket id of sever and client
	struct sockaddr_in severaddr;	// ip address of server
	
	char buf[MAXLINE];
	int n;
	
	// socket init
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create sever socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	
	memset(&severaddr, 0, sizeof(severaddr));
	severaddr.sin_family = AF_INET;
	severaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	severaddr.sin_port = htons(6666);
	
	// bind socket id
	if (bind(listenfd, (struct sockaddr*)&severaddr, sizeof(severaddr)) == -1) {
		printf("bind sever socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	
	// linsten socket id
	if (listen(listenfd, 10) == -1) {
		printf("listen sever socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	
	printf("=====Waiting for client's request===\n");
	
	while(1) {
		if ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
			printf("accept sever socket error: %s(errno: %d)\n", strerror(errno), errno);
			return 0;
		}
		
        char recvMsg[100];
		ssize_t readLen = read(connfd, recvMsg, sizeof(recvMsg));
        if (readLen < 0) {
            printf("read error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
		recvMsg[9] = '\0';
		printf("readLen: %d, recvMsg: %s\n", readLen, recvMsg);

        sleep(5);
        recvMsg[1] = '9';
        ssize_t writeLen = write(connfd, recvMsg, sizeof(recvMsg));
        if (readLen < 0) {
            printf("write error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
		
		close(connfd);
		return 0;
	}
	//close(listenfd);
	return 0;
	
}
