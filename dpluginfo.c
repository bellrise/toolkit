/* Utility to read the metadata from dplug plugins.
   Copyright (c) 2023 bellrise */

#include <dlfcn.h>
#include <dplug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readplugin(const char *lib);

int main(int argc, char **argv)
{
	if (argc < 2 || (argc > 1 && !strcmp(argv[1], "--help"))) {
		puts("usage: dpluginfo <plugin>");
		return 0;
	}

	return readplugin(argv[1]);
}

void print_method_table(struct dplug_decl *decl)
{
	struct dplug_method *method;
	int i;

	printf("method table:\n");

	i = 0;
	while (1) {
		method = &decl->d_methods[i++];
		if (!method->e_name)
			break;
		printf("  %s\n", method->e_name);
	}
}

void printinfo(struct dplug_decl *decl)
{
	if (!decl->d_name)
		printf("malformed plugin, missing name\n");
	else
		printf("name: %s\n", decl->d_name);

	if (decl->d_desc)
		printf("desc: %s\n", decl->d_desc);
	printf("version: %d\n", decl->d_version);
	printf("has open: %s\n", decl->d_open ? "yes" : "no");
	printf("has close: %s\n", decl->d_close ? "yes" : "no");

	if (decl->d_methods[0].e_name)
		print_method_table(decl);
}

int readplugin(const char *lib)
{
	struct dplug_decl *decl;
	char path[256];
	void *handle;

	if (lib[0] != '.' && lib[0] != '/')
		snprintf(path, 256, "./%s", lib);
	else
		snprintf(path, 256, "%s", lib);

	handle = dlopen(path, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "error: failed to open plugin at `%s`\n", path);
		return 1;
	}

	/* Current decl tab is under __dplug_v1_decl. */
	decl = dlsym(handle, "__dplug_v1_decl");
	if (!decl) {
		fprintf(stderr, "error: failed to find plugin metadata\n");
		return 1;
	}

	printinfo(decl);

	dlclose(handle);
	return 0;
}
