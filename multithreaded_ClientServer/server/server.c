
#include "chatHeader.h"

static int runningThread = 0 ; 	// which thread is running status

int main()
{
	/* Defining Variables */
	int sockfd = -1 ;
	int clientsockfd = -1 ; 
	int *new_sock = NULL ;

	struct sockaddr_in client_addr; /* Internet Specific(AF_INET) client socket address stucture */
	struct sockaddr_in server_addr; /* Internet Specific(AF_INET) server socket address stucture */
	memset(&client_addr,0,sizeof(client_addr));
	memset(&server_addr,0,sizeof(server_addr));

	int sin_size = 0 ;	// used to finding size of struct sockaddr_in

	/* socket(), Get the Socket file descriptor */
	if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		printf("\tERROR : Failed to obtain Socket Descriptor ( error = %s )\n", strerror(errno));
		return 1;
	}
	else 
		printf("[Server] Obtaining socket descriptor successfully.\n");


	/* Fill the server socket address struct */
	server_addr.sin_family = AF_INET;		// Protocol Family
	server_addr.sin_port = htons(PORT); 		// Port number
	server_addr.sin_addr.s_addr = INADDR_ANY; 	// AutoFill local address
	bzero(&(server_addr.sin_zero), 8); 		// Flush the rest of struct
		/*      IPv4 AF_INET sockets:
                struct sockaddr_in {
                        short            sin_family;   // e.g. AF_INET, AF_INET6
                        unsigned short   sin_port;     // e.g. htons(3490)
                        struct in_addr   sin_addr;     // see struct in_addr, below
                        char             sin_zero[8];  // zero this if you want to
                        };

                struct in_addr {
                        unsigned long s_addr;          // load with inet_pton()
                        };
 	       */

	/* bind(), Bind a special Port */
	if( bind( sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1 ) // cast server_addr to pointer type (struct sockaddr*)
	{
		printf("\tERROR : Failed to Bind Port ( errno = %s )\n", strerror(errno) );
		close(sockfd);
		return 1;
	}
	else 
		printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);


	/* listen(), Listen remote connection/calling */
	if( listen( sockfd, BACKLOG ) == -1 )	// BACKLOG : limlit to pending connections in queue 
	{
		printf("\tERROR : Failed to Listen Port ( error = %s )\n", strerror(errno) );
		close(sockfd);
		return 1;
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);

	sin_size = sizeof(struct sockaddr_in);

	/* accept(), Wait a connection, and obtain a new socket file despriptor for single connection */
	while( clientsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size) )
	{
		printf("[Server] Server has got connected from %s.\n", inet_ntoa(client_addr.sin_addr)); // convert Inet no. to ASCII

	        pthread_t client_thread;
	        new_sock = malloc(1);
        	*new_sock = clientsockfd;

        	if( pthread_create( &client_thread , NULL ,  conn_handler , (void*)new_sock) < 0)
        	{
            		printf("\tError : could not create thread ( error = %s )\n", strerror(errno) );
            		close(clientsockfd);
			close(sockfd);
			return 1;
        	}

               	printf(" > Handler assigned\n");
	}
	if( clientsockfd < 0 )
	{
		printf("\tERROR : Obtaining new Socket Despcritor ( error = %s )\n", strerror(errno) );
		close(sockfd);
		return 1;
	}

	close(sockfd);
	return 0;
}


void *conn_handler( void *socket_desc )
{
	//Get the socket descriptor
    	int sock = *(int*)socket_desc;
	int no_of_bytes = -1 ;	// used in send(), recv()
	int tno = 0 ;

	char msg[LENGTH];
	memset(msg,0,LENGTH);

	runningThread++;
	tno = runningThread;

	while( ( no_of_bytes = recv( sock, msg, LENGTH, 0 ) ) > 0 )
	{
		printf("\nMessage got from client(%d): %s\n",tno,msg);
		if( strncmp(msg,"end",3) == 0 )
                        break;
             
		memset(msg,0,LENGTH);
                printf("\nEnter data to be send to client(%d) : ",tno);
                scanf(" %[^'\n']s",msg);

                no_of_bytes = send( sock, msg, LENGTH, 0);
                if( no_of_bytes == -1 )
                {
                        printf("\tError in sending\n");
                        break;
                }            
		if( strncmp(msg,"end",3) == 0 )
                        break;
   
                memset(msg,0,LENGTH);
	}

	printf("Terminating connection with client(%d)\n",tno);	
    	free(socket_desc);
	pthread_exit(NULL);
}

//############################################################################################################
