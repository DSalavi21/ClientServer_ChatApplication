
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>             // bzero(), memset()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>         // sockaddr_in structure to handle internet addresses
#include <pthread.h>            //for threading , link with lpthread
//          
#include <arpa/inet.h>          // htons() function converts the unsigned short integer hostshort from host byte order to network byte order

#define PORT 8888 
#define LENGTH 512 

#define BACKLOG 5               // used in server, to limit pending connection

//the server side thread routne
void *conn_handler(void *);

