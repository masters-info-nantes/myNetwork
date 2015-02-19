#include "LinkedListString.h"
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#ifndef NULL
#define NULL 0
#endif

LinkedListString* makeLinkedListString() {
	LinkedListString* l = (LinkedListString*)malloc(sizeof(LinkedListString));
	l->size = 0;
	l->first = 0;
	l->last = 0;
	return l;
}

void deleteLinkedListString(LinkedListString* l, bool withContent) {
	StringEntry* current = l->first;
	StringEntry* next;
	while(current != NULL) {
		next = current->next;
		if(withContent) {
			free(current->content);
		}
		free(current);
		current = next;
	}
	free(l);
}

void addString(LinkedListString* l, char* s) {
	StringEntry* e = (StringEntry*)malloc(sizeof(StringEntry));
	e->back = NULL;
	e->next = NULL;
	e->content = clonestr(s);
	if(getSize(l) == 0) {
		l->last = e;
		l->first = e;
		l->size++;
	} else {
		e->back = l->last;
		l->last->next = e;
		l->last = e;
		l->size++;
	}
}

void concatLastString(LinkedListString* l, char* s) {
	if(l->size > 0) {
		char* back = l->last->content;
		l->last->content = concat(back,s);
	} else {
		addString(l,s);
	}
}

char* getString(LinkedListString* l, int index) {
	if(index == 0) {
		return l->first->content;
	} else if(index == (l->size-1)) {
		return l->last->content;
	} else if(index < l->size && index > 0) {
		StringEntry* cur = l->first;
		for(int i=1;i<=index;i++) {
			cur = cur->next;
		}
		return cur->content;
	}
	return 0;
}

int getSize(LinkedListString* l) {
	return l->size;
}

char* removeFirst(LinkedListString* l) {
	StringEntry* remove = l->first;
	char* ret = remove->content;
	l->first = remove->next;
	l->first->back = NULL;
	l->size--;
	free(remove);
	return ret;
}

char* removeLast(LinkedListString* l) {
	StringEntry* remove = l->last;
	char* ret = remove->content;
	l->last = remove->back;
	l->last->next = NULL;
	l->size--;
	free(remove);
	return ret;
}

char* toCharRequest(LinkedListString* l) {
	char *ret = "", *tmp;
	StringEntry* cur = l->first;
	while(cur != 0) {
		tmp = ret;
		ret = concat(ret,cur->content);
		free(tmp);
		tmp = ret;
		ret = concat(ret,"\n");
		free(tmp);
	}
	return ret;
}
