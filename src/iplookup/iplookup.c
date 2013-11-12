#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

void iplookup(const char * const addr)
{
	char host[NI_MAXHOST];
	struct sockaddr_in in = { .sin_family = AF_INET };
	struct sockaddr_in6 in6 = { .sin6_family = AF_INET6 };
	struct sockaddr * p;
	int r, size;

	if(strchr(addr, ':')) {
		inet_pton(AF_INET6, addr, &in6.sin6_addr);
		p = (struct sockaddr *) &in6;
		size = sizeof(in6);
	} else {
		inet_pton(AF_INET, addr, &in.sin_addr);
		p = (struct sockaddr *) &in;
		size = sizeof(in);
	}

	r = getnameinfo(p, size, host, NI_MAXHOST, NULL, 0, 0);
	if(r) {
		printf("%s: %s\n", addr, gai_strerror(r));
		return;
	}
	printf("%s: %s\n", addr, host);
}

int main(int argc, char *argv[])
{
	int i = 0;

	if(argc < 2) {
		printf("Usage: %s <ip address> [ip address...]\n", argv[0]);
		return 1;
	}

	for(i = 1; i < argc; ++i)
		iplookup(argv[i]);

	return 0;
}
