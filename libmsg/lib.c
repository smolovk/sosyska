#include <stdio.h>

int i = 0;

void smsg(char* type) {
	printf("[libmsg.so] Caught a message, type: %s\n", type);
}
