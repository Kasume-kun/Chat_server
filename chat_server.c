#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in address = {
		AF_INET,
		htons(5555),
		0
	};
	
	bind(sockfd, &address, sizeof(address));

    	listen(sockfd, 10);

    	int clientfd = accept(sockfd, 0, 0);

	connect(sockfd, &address, sizeof(address));

	struct pollfd fds[2] = {
		{
			0, 		//File descriptor (stdin - 0)
			POLLIN,		//Checks if anything is ready to be read in stdin fd
			0
		},
		{
			clientfd,
			POLLIN,
			0
		}
	};

	while(1)
	{
		char buffer[256] = {0};
	
		poll(fds, 2, 50000); //(array of fds, no. of fds, timeout) 
	
		if(fds[0].revents & POLLIN)
		{
			read(0, buffer, 255);
			send(clientfd, buffer, 255, 0);
		}
		else if(fds[1].revents & POLLIN)
		{
			if(recv(clientfd, buffer, 255, 0) == 0)
				return 0;
			
			printf("%s\n",buffer);
		}
	}
	return 0;
}
