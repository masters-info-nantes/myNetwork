#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* concat(char* s1, char* s2) {
	char* both = (char*) malloc((strlen(s1) + strlen(s2) + 1) * sizeof(char));
	sprintf(both,"%s%s",s1,s2);
	return both;
}

char* clonestr(char* s) {
	char* clone = (char*) malloc((strlen(s)+1) * sizeof(char));
	sprintf(clone,"%s",s);
	return clone;
}

bool sameString(char* s1, char* s2, int nbCharToCompare) {
	for(int i=0;i<nbCharToCompare;i++) {
		if(s1[i] != s2[i])
			return false;
	}
	return true;
}
