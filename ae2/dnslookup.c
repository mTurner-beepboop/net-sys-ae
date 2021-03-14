#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

//INET_ADDRSTRLEN and INET6_ADDRSTRLEN defined in netinet/in.h

int main(int argc, char *argv[]){
	//Check if arguments are correct firstly
	if (argc < 2){
		printf("Usage: %s <hostname> <hostname> ...\n", argv[0]);
		return 1;
	}
	
	char *buf4 = malloc(INET_ADDRSTRLEN);
	char *buf6 = malloc(INET6_ADDRSTRLEN);
	struct addrinfo hints, *ai, *ai0;
	int j;
	//Perform dns lookup on each name
	for (int i = 1; i < argc; i++){
		//Get avaialbe hosts for current name
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		if ((j = getaddrinfo(argv[i], "53", &hints, &ai0)) != 0){
			printf("Unable to lookup %s: %s\n", argv[i], gai_strerror(j)); 
			continue; //Try next arg if available
		}
		
		//Iterate through available addresses
		for (ai = ai0; ai != NULL; ai = ai->ai_next){
			if (ai->ai_family == AF_INET){
				//IPv4 addr
				struct sockaddr_in *ipv4_in = (struct sockaddr_in *) ai->ai_addr;
				if (NULL == (inet_ntop(AF_INET, &(ipv4_in->sin_addr), buf4, INET_ADDRSTRLEN))){	
					//Error
					printf("Error in IPv4 Lookup for %s", argv[i]); 
				} else {
					//Success
					printf("%s IPv4 %s\n", argv[i], buf4);
				}
			} else {
				//IPv6 addr				
				struct sockaddr_in6 *ipv6_in = (struct sockaddr_in6 *) ai->ai_addr;
				if (NULL == (inet_ntop(AF_INET6, &(ipv6_in->sin6_addr), buf6, INET6_ADDRSTRLEN))){	
					//Error
					printf("Error in IPv6 Lookup for %s", argv[i]);
				} else {
					//Success
					printf("%s IPv6 %s\n", argv[i], buf6);
				}
			}
		}
	}
	freeaddrinfo(ai0);
	free(buf4);
	free(buf6);
	
	return 0;
}
