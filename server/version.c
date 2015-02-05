#include "version.h"
#include <stdio.h>

#define PROGRAM_NAME "DNS"
#define PROGRAM_VERSION "0.0.1a"
#define PROGRAM_AUTHORS "Franck Boncler & Anthony Pena"

void modeVersion() {
	printf("%s - Version %s\n",PROGRAM_NAME, PROGRAM_VERSION);
	printf("Developped by : %s\n",PROGRAM_AUTHORS);
}
