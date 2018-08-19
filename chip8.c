#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} boolean;

int memory_size = 4096;
unsigned char memory[4096];

boolean loadGame(const char *filename){
	FILE *fp = fopen(filename, "r");
	if (fp == NULL){
		printf("Error: file %s could not be opened\n", filename);
		return false;
	}

	fseek(fp, 0L, SEEK_END);
	int fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	printf("Filesize: %i\n", fsize);

	unsigned char *buffer = malloc(sizeof(char)*fsize);
	if (buffer == NULL){
		fputs("Memory error", stderr);
		return false;
	}

	int size = 1;
	size_t ret = fread(buffer, size, fsize, fp);
	if (ret != fsize){
		fputs("Error reading file", stderr);
	}

	if ((memory_size - 512) > fsize){ // CHIP-8 reserved memory ends at 512 ie. 0x200
		for(int i = 0; i < fsize; fsize++){
			memory[i + 512] = buffer[i];
		}
	}
	else {
		fputs("Error: ROM too big for memory", stderr);
	}

	fclose(fp);
	free(buffer);
	
	return true;
}


int main(int argc, char **argv){
	boolean success = loadGame(argv[1]);
	
	if (!success){
		exit(1);
	}

	return 0;
}