#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE	42	// In fact, only 2 bytes are needed
#define MAGIC_CHAR	'@'	// Default splitting char

int main(int argc, char**argv)
{
	if (argc >= 2)
	{
		printf("*** DNSChannel 0.1 - Consumer ***\n");
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
		servaddr.sin_port = htons(atoi(argv[1]));
	
		// Bind socket to address
		err = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if (err < 0)
		{
			perror("Can't bind socket : ");
			return -1;
		}

		// Start the magic loop
		int code = 0;
		char splitChar;
		if (argc >= 3)
			splitChar = atoi(argv[2]);
		else
			splitChar = MAGIC_CHAR;
		printf("Listening on port %s, waiting for message(s) ...\n", argv[1]);
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
					printf("%c", (char) msg[1]);
			}
		}
	}
	else
	{
		printf("Usage: consumer port_to_listen [split_char]\n");
		return 0;
	}
	return 42;
}
