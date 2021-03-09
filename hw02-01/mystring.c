#include <stdlib.h>
#include <stdio.h>

size_t mystrlen(const char *str) {
	const char *iter = NULL;
	for(iter = str; *iter != '\0'; ++iter);
	return iter - str;
}

char *mystrchr(const char *str, int c) {
	char *iter = (char *)str;
	for(; (*iter) != '\0' && (*iter) != c; ++iter);
	if(*iter == '\0') return NULL;
	return iter;
}

char *mystrrchr(const char *str, int c){
	char *iter = (char *) str;
	for(;*iter != '\0'; ++iter);
	for(;iter != str && *iter != c; --iter);
	if(iter == str && *iter != c) return NULL;
	return iter;
}

size_t mystrspn(const char *str, const char *accept){
	const char *iter =  str;
	for(;mystrchr(accept, *iter) != NULL; ++iter);
 	return iter - str;
}

size_t mystrcspn(const char *str, const char *reject){
	const char *iter =  str;
	for(;*iter != '\0' && mystrchr(reject, *iter) == NULL; ++iter);
 	return iter - str;
}

char *mystrpbrk(const char *str, const char *accept){
	char *iter =  (char *)str;
	for(;*iter != '\0' && mystrchr(accept, *iter) == NULL; ++iter);
	if(*iter == '\0') return NULL;
 	return iter;
}

char *mystrstr(const char *haystack, const char *needle){
	char *iter = (char *)haystack;
	for(;*iter != '\0'; ++iter) {
		if(*iter == needle[0]) {
			int flag = 0;
			for(int i = 1; needle[i] != '\0'; ++i ) {
				if(iter[i] != needle[i]){
					flag = 1;
					break;
				}
			}
			if(flag) {
				continue;
			}else {
				return iter;
			}
		}

	}
	return NULL;
}

char *mystrtok(str,delim)char * str; const char *delim; {
	static char *tmp = NULL;
	if(str != NULL) {
		tmp = str;
	}
	if(tmp == NULL) return NULL;

	char *iter = mystrstr(tmp, delim);
	char *ret = tmp;
	while(*ret == ' ') ++ret;
	if(iter == NULL) {
		tmp = NULL;
		return ret;
	}
	*iter = '\0';
	tmp = iter + mystrlen(delim);

	return ret;
}