#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <malloc.h>

void readBEZ(char* filename){
	if (filename == NULL) {
		fprintf(stderr, "ERROR: No file specified for obj command\n");
		return;
	}

	FILE* finput = fopen(filename, "r");
	if (finput == NULL) {
		fprintf(stderr, "ERROR: cannot open bez file '%s'\n", filename);
		return;
	}

	char line[19];
	char* pch;
	while (fgets(line, sizeof(line), finput)) {
		if ((pch = strtok(line, " \t \n")) == NULL) continue;

	}

}