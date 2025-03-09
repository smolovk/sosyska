#include <stdio.h>

int i = 0;
void (*sendMsg)(char* type);

void sinit(void (*msg)(char* type)) {
	printf("[libtest.so]: init\n");
	sendMsg = msg;
}

void stick() {
	printf("[libtest.so]: number: %d\n", i);
	if (i%2==0 && i!=0)
		sendMsg("even");
	i++;
}

void sclose() {
	printf("[libtest.so]: Bye-bye!\n");
}
