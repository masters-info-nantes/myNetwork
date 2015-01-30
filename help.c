#include "help.h"
#include <stdio.h>

void modeHelp() {
	printf("USAGE: dns [-d|--daemon]\n");
	printf("USAGE: dns --help|-h\n\n");

	printf("   -d, --daemon\n");
	printf("        Program will be start in background, and daemon PID will be print.\n");
	printf("   -h, --help\n");
	printf("        Show this help.\n");
	printf("   -v, --version\n");
	printf("        Show version information.\n");
}
