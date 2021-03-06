#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void error(char* msg) { perror(msg); exit(1); }

int main(int argc, char** argv) {
    int listenfd;
    int connfd;
    int portno;
    int clientlen;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    struct hostent* hostp;
    char buf[BUFSIZE];
    char* hostaddrp;
    int optval;
    int n;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]); 
        exit(1);
    }
    portno = atoi(argv[1]);
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        error("ERROR opening socket");

    optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        error("ERROR on binding");
    if (listen(listenfd, 5) < 0)
        error("ERROR on binding"); 
    clientlen = sizeof(clientaddr);
    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        if (connfd < 0)
            error("ERROR on accept");
        hostp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL) error("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL) error("ERROR on inet_ntoa\n");
        
        bzero(buf, BUFSIZE);
        n = read (connfd, buf, BUFSIZE);
        if (n < 0) error ("ERROR reading from socket");
        printf("server received %d bytes: %s", n, buf);

        n = write(connfd, buf, BUFSIZE);
        if (n < 0) error("ERROR writing to socket");

        close(connfd);
    }
}
