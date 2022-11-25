# Build all the tools using clang. If you want to change the
# compiler, just run:
#	sed -i 's/clang/gcc/g' buildfile

@default
	echo 'usage: build <program>'

@dbytes
	clang -Wall -Wextra -o /usr/local/bin/dbytes dbytes.c -fsanitize=address

@resolve
	clang -Wall -Wextra -o /usr/local/bin/resolve resolve.c
