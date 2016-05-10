#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int (*o_open)(const char*, int oflag) = NULL;
char* (*o_getenv)(const char *) = NULL;

char* getenv(const char *name)
{
	if(!o_getenv)
		o_getenv = dlsym(RTLD_NEXT, "getenv");

	printf("we are in getenv of hooker\n");
	return o_getenv(name);
}

int open(const char *path, int oflag, ...)
{
	char real_path[PATH_MAX];
	if(!o_open)
		o_open = dlsym(RTLD_NEXT, "open");

	printf("we are in open() of hooker\n");
	return o_open(path, oflag);
}

