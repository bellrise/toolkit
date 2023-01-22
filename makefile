CC ?= clang

default:
	@echo 'usage: make <program>'

dbytes:
	@$(CC) -Wall -Wextra -o /usr/local/bin/dbytes dbytes.c

resolve:
	@$(CC) -Wall -Wextra -o /usr/local/bin/resolve resolve.c

sdc:
	@$(CC) -Wall -Wextra -o /usr/local/bin/sdc sdc.c

dpluginfo:
	@$(CC) -Wall -Wextra -o /usr/local/bin/dpluginfo -ldl dpluginfo.c
