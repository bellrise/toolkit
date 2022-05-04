/* Utility similar to default xxd, without vim.
   Because of the formatting of this program, you can easily parse it
   using awk. For example, to print only the hex bytes:

   $ dbytes <file> | awk -F' : ' '{ print $2 }'

   Copyright (c) 2022 bellrise */

#include <string.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char **argv)
{
	unsigned char buf[16] = {0};
	size_t bytes_read;
	size_t line;
	FILE *f;

	/* RSD 3/2: required argument --help */
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		puts("usage: dbytes <file>");
		return EINVAL;
	}

	if (!(f = fopen(argv[1], "r"))) {
		printf("No such file or directory: %s\n", argv[1]);
		return ENOENT;
	}

	/* Each line in dbytes represents 16 bytes. To not load the whole
	   file into memory at once, which could be problematic we only
	   fread 16 bytes at a time. */
	line = 0;
	while ((bytes_read = fread(buf, 1, 16, f))) {
		printf("%08zx : ", (line++) * 16);

		for (size_t i = 0; i < 16; i++) {
			i >= bytes_read ? printf("  ") : printf("%02hhx", buf[i]);
			if (i % 2 != 0)
				fputc(' ', stdout);
		}

		printf(": ");
		for (size_t i = 0; i < bytes_read; i++)
			fputc(buf[i] >= 0x20 && buf[i] <= 0x7e ? buf[i] : '.', stdout);
		fputc('\n', stdout);
	}
}
