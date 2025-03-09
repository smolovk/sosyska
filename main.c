#include <dlfcn.h> // dlopen....
#include <dirent.h> // directories
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "./vector/vector.h"

static int keepRunning = 1;
static StringVector msgBus;

int getlibs(char *directory, char* libs[]);
void* loadLib(char* libsFolder, char* file);

void catch() {
	keepRunning = 0;
}

void handleMsg(char* type) {
	pushBack(&msgBus, type);
}

int main() {
	initVector(&msgBus, 2);

	char* libsFolder = "./libs/";


	char* files[100];
	int size = getlibs("./libs", files);
	void *handles[size];
	for (int i = 0; i<size; i++) {
		char* file = files[i];
		printf("Found %s, trying to load...\n", file);

		void* handle = loadLib(libsFolder, file);
		if(!handle) {
			return 1;
		}
		handles[i] = handle;
		
		void (*initFunc)(void (*sendMsg)(char* type));
		initFunc = dlsym(handle, "sinit");
		if (initFunc) {
			initFunc(handleMsg);
		}
	}

	signal(SIGINT, catch);

	while(keepRunning) {
		for (int i = 0; i<size; i++) {
			void* handle = handles[i];

			void (*tickFunc)();
			tickFunc = dlsym(handle, "stick");
			if (tickFunc) {
				tickFunc();
			}
		
		}

		if (msgBus.size > 0) {
			for(int i = msgBus.size-1; i>=0; i--) {
				char* msg = popBack(&msgBus);

				for (int j = 0; j<size; j++) {
					void* handle = handles[j];

					void (*msgFunc)(char* type);
					msgFunc = dlsym(handle, "smsg");
					if (msgFunc) {
						msgFunc(msg);
					}
				}

				free(msg);
			}
		}

		sleep(1);
	}

	freeVector(&msgBus);

	for (int i = 0; i<size; i++) {
		void* handle = handles[i];

		void (*closeFunc)();
		closeFunc = dlsym(handle, "sclose");

		if (closeFunc) {
			closeFunc();
		}

		dlclose(handles[i]);
	}

	return 0;
}

void* loadLib(char* libsFolder, char* file) {
		size_t pathSize = strlen(libsFolder)+strlen(file)+1;
		char* path = malloc(pathSize);
		if(!path) {
			printf("Failed to allocate memory\n");
			return NULL;
		}
		snprintf(path, pathSize, "%s%s", libsFolder, file);
		void* handle = dlopen(path, RTLD_LAZY);
		free(path);
		if (!handle) {
			printf("Loading library %s has failed.\nError: %s", file, dlerror());
			return NULL;
		}

		return handle;
}


int getlibs(char *directory, char* libs[]) {
	DIR *d;
	struct dirent *dir;
	d = opendir(directory);
	int i = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
				libs[i] = dir->d_name;
				i++;
			}
		}
		closedir(d);
	}
	return i;
}
