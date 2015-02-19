#ifndef __LINKEDLISTSTRING__
#define __LINKEDLISTSTRING__
#include <stdbool.h>

typedef struct StringEntry StringEntry;
struct StringEntry {
	StringEntry* back;
	StringEntry* next;
	char* content;
};

typedef struct {
	StringEntry* first;
	StringEntry* last;
	int size;
} LinkedListString;

LinkedListString* makeLinkedListString();
void deleteLinkedListString(LinkedListString* l, bool withContent);

void addString(LinkedListString* l, char* s);
void concatLastString(LinkedListString* l, char* s);
char* getString(LinkedListString* l, int index);
int getSize(LinkedListString* l);
char* removeFirst(LinkedListString* l);
char* removeLast(LinkedListString* l);
char* toCharRequest(LinkedListString* l);
#endif /* __LINKEDLISTSTRING__ */
