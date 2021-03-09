#include <stdlib.h>
#include <stdio.h>
char *mystrchr(const char *str, int c) {
	char *iter = (char *)str;
	for(; (*iter) != '\0' && (*iter) != c; ++iter);
	
	if(*iter == '\0') return NULL;
	
	return iter;

}

