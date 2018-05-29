
#include "chatHeader.h"

int main()
{
	/* Variable Definition */
	int sockfd = -1 ; 
	int no_of_bytes = -1 ;	

	struct sockaddr_in server_addr; // Internet Specific(AF_INET) server socket address stucture 
	memset(&server_addr,0,sizeof(server_addr));

	char msg[LENGTH]; 
	memset(msg,0,LENGTH);
	
	/* socket(), Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("\tERROR : Failed to obtain Socket Descriptor! ( error = %s )\n", strerror(errno) );
		return 1;
	}

	/* Fill the socket address struct */
	server_addr.sin_family = AF_INET; 
	server_addr.sin_port = htons(PORT); 
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // converts readable-ip into network form
	bzero(&(server_addr.sin_zero), 8);

	/* connect(), Try to connect the server */
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("\tERROR : Failed to connect to the host! ( error = %s )\n", strerror(errno) );
		close(sockfd);
		return 1;
	}
	else 
		printf("[Client] Connected to server at port %d...ok!\n", PORT);	

	while(1)
   	{
		memset(msg,0,LENGTH);
        	printf("Enter data to be send to server: ");
        	scanf(" %[^'\n']s",msg);
        	if( strncmp(msg,"end",3) == 0 )
            		break;

        	no_of_bytes = send( sockfd, msg, LENGTH, 0 );
        	if( no_of_bytes == -1 )
        	{
            		printf("\tError in sending\n");
            		exit(1);
        	}

		memset(msg,0,LENGTH);
        	no_of_bytes = recv( sockfd , msg, LENGTH, 0 );
        	if( no_of_bytes == -1 )
        	{
            		printf("\tError in receiving\n");
            		exit(1);
        	}
		      if( strncmp(msg,"end",3) == 0 )
                        break;

        	printf("Message got from server is : %s\n",msg);
    	}
	close(sockfd);
	return 0;
}
