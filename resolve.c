/* Resolve a domain name & show all results from <netdb.h>.
   Copyright (c) 2022 bellrise */

#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	struct hostent *resolved;

	/* RSD 3/2: required argument --help */
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		puts("usage: resolve <domain>");
		return 1;
	}

	resolved = gethostbyname(argv[1]);
	if (!resolved || !resolved->h_addr_list[0])
		return 1;

	printf("%s\n", inet_ntoa(**(struct in_addr **) resolved->h_addr_list));
}
