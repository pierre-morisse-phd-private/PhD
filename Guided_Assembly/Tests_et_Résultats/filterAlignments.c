#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char* argv[]) {
	FILE* f = fopen(argv[1], "r");

	char tok[MAX_LINE];
	char* line;
	char* t;
	int alLen;
	float id;

	int lmin = atoi(argv[2]);
	int lmax = atoi(argv[3]);
	float idmin = atoi(argv[4]);
	
	fprintf(stderr, "idmin : %f\n", idmin);

	while(fgets(tok, MAX_LINE, f) != NULL) {
		line = malloc((strlen(tok) + 1) * sizeof(char));
		strncpy(line, tok, strlen(tok) + 1);
		t = strtok(tok, "\t");
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		id = atof(t);
		t = strtok(NULL, "\t");
		alLen = atoi(t);
		t = strtok(NULL, "\t");
		if (id >= idmin && lmin <= alLen && alLen <= lmax) {
			printf("%s", line);
			free(line);
		}
	}
}
