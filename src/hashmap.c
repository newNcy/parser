#include "hashmap.h"
#include <stdio.h>


long hash(const char * str,int Max)
{
	if (!str) return 0;
	int h = 0;
	char * p = str;
	while (*p) {
		h = (h*15+*p)%Max;
		p++;
	}
	return h;
}

int main ()
{
	const char * t[] = {
		"id",
		"int",
		"float",
		"double",
		"long"
	};
	for (int i = 0 ; i < sizeof(t)/sizeof(char*); i++) {
		printf("%d\n",hash(t[i],1024));
	}

}
