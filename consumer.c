#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_SIZE	42 // In fact, only 2 bytes are needed
#define MAGIC_CHAR	'@' // Char used to split messages

int main(int argc, char**argv)
{
	// Variables
	int sockfd, err;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	char msg[BUFFER_SIZE];

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("Can't create socket : ");
		return -1;
	}
	
	// Setup servaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(1337);
	
	// Bind socket to address
	err = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (err < 0)
	{
		perror("Can't bind socket : ");
		return -1;
	}

	// Start the magic loop
	int code = 0;
	while (1)
	{
		len = sizeof(cliaddr);
		err = recvfrom(sockfd, msg, BUFFER_SIZE, 0, (struct sockaddr *) &cliaddr, &len);
		if (err > 0)
		{
			if (msg[0] == MAGIC_CHAR)
			{
				if (!code)
				{
					code = 1;
					printf("*** NEW MESSAGE ***\n");
				}
				else
				{
					code = 0;
					printf("\n*** END OF MESSAGE ***\n");
				}
			}
			else if (code)
			{
				printf("%c", (char) msg[1]);
			}
		}
	}
	
	return 42;
}
