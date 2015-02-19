#include "version.h"
#include <stdio.h>
#include "common.h"

void modeVersion() {
	printf("%s - Version %s\n",PROGRAM_NAME, PROGRAM_VERSION);
	printf("Developped by : %s\n",PROGRAM_AUTHORS);
}
