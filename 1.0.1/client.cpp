#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MAXLINE 4096

int main(int argc, char** argv) {
	int socketfd, n;
	char recvline[MAXLINE], sendline[MAXLINE];
	
	struct sockaddr_in severaddr;	// ip address of server
	
	if (argc != 2) {
		printf("usage: ./client <ipadress>\n");
		return 0;
	}
	
	
	// socket init
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create client socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	
	memset(&severaddr, 0, sizeof(severaddr));
	severaddr.sin_family = AF_INET;
	severaddr.sin_port = htons(6666);
	
	if (inet_pton(AF_INET, argv[1], &severaddr.sin_addr) < 0) {
		printf("inet_pton error for: %s\n", argv[1]);
		return 0;
	}

	if (connect(socketfd, (struct sockaddr*)&severaddr, sizeof(severaddr)) < 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
    
    // add the time out
    struct timeval setTimeVal;
    setTimeVal.tv_sec = 2;
    setTimeVal.tv_usec = 0;
    if (setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, &setTimeVal, sizeof(setTimeVal))) {
        printf("set socketopt error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &setTimeVal, sizeof(setTimeVal))) {
        printf("set socketopt error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    ssize_t writenlen;
    char sendMsg[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '\0'};
    int count = 0;
    int writeLen = write(socketfd, sendMsg, sizeof(sendMsg));
    if (writeLen < 0) {
        printf ("write error ：%s(errno ：%d) \n", strerror(errno),errno);
        close (socketfd) ;
        return 0;
    }else
        printf ("write sucess, writelen ：%d, sendMsg ：%s\n ", writeLen, sendMsg);
    
    char readMsg[10]={0};
    int readlen= read(socketfd, readMsg,sizeof(readMsg));
    if (readlen < 0) {
        printf ("read error :%s(errno ： %d) \n ", strerror(errno),errno);
        close (socketfd);
        return 0;
    }else{
        readMsg[9]='\0';
        printf(" read sucess, readlen : %d, readMsg ：%s\n ", readlen, readMsg);
    }
    close (socketfd);
    return 0;
    
}


