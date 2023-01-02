/* Utility to read SDC files.
   Copyright (c) 2023 bellrise */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sdc.h>

int readsdc(FILE *f);
int readentry(FILE *f, int padding_level);


int main(int argc, char **argv)
{
	FILE *f;
	int err;

	if (argc > 1 && !strcmp(argv[1], "--help")) {
		puts("usage: sdc <file>");
		return 0;
	}

	if (argc < 2) {
		f = stdin;
	} else {
		if (!(f = fopen(argv[1], "rb"))) {
			puts("error: failed to open file");
			return 1;
		}
	}

	err = readsdc(f);
	fclose(f);

	return err;
}

void pad(int padding_level)
{
	while (padding_level--)
		fputs("  ", stdout);
}

int readarray(FILE *f, int padding_level)
{
	struct sdc_array array;

	fread(&array, 1, sizeof(array), f);
	printf("array[%d]\n", array.e_elems);

	for (int i = 0; i < array.e_elems; i++)
		readentry(f, padding_level + 1);
	return 0;
}

int readentry(FILE *f, int padding_level)
{
	struct sdc_entry entry;
	char namebuf[SDC_MAXNAME];
	unsigned long int_value;
	char *name;
	char *str;

	pad(padding_level);

	fread(&entry, 1, sizeof(entry), f);

	if (entry.e_flags & SDC_ENAMED) {
		if (entry.e_flags & SDC_ELONGNAME)
			name = malloc(SDC_MAXLONGNAME);
		else
			name = namebuf;

		char c;
		int i = 0;
		do {
			c = fgetc(f);
			name[i++] = c;
		} while (c);

		printf("%s: ", name);
	}

	int_value = 0;

	switch (entry.e_type) {
	case SDC_NULL:
		printf("null\n");
		break;
	case SDC_INT:
		fread(&int_value, 1, 4, f);
		printf("int=%d\n", (int) int_value);
		break;
	case SDC_UINT:
		fread(&int_value, 1, 4, f);
		printf("uint=%d\n", (unsigned int) int_value);
		break;
	case SDC_LONG:
		fread(&int_value, 1, 8, f);
		printf("long=%ld\n", (long) int_value);
		break;
	case SDC_ULONG:
		fread(&int_value, 1, 8, f);
		printf("ulong=%lu\n", (unsigned long) int_value);
		break;
	case SDC_BOOL:
		fread(&int_value, 1, 1, f);
		printf("bool=%s\n", int_value ? "true" : "false");
		break;
	case SDC_STRING:
		str = malloc(entry.e_size);
		fread(str, 1, entry.e_size, f);
		printf("str='%s'\n", str);
		free(str);
		break;
	case SDC_ARRAY:
		readarray(f, padding_level);
		break;
	default:
		return 1;
	}

	if (entry.e_flags & SDC_ELONGNAME)
		free(name);

	return 0;
}

int readsdc(FILE *f)
{
	struct sdc_header header;

	if (fread(&header, 1, 3, f) < 3) {
		puts("error: file too small");
		return 1;
	}

	if (memcmp(&header, SDC_MAGIC, 3)) {
		puts("error: not an sdc file, invalid magic");
		return 1;
	}

	fseek(f, 0, SEEK_SET);
	fread(&header, 1, sizeof(header), f);

	if (header.h_version != SDC_VERSION) {
		printf("error: unsupported version, expected %d but found %d\n",
				SDC_VERSION, header.h_version);
		return 1;
	}

	for (int i = 0; i < header.h_entries; i++)
		readentry(f, 0);

	return 0;
}
