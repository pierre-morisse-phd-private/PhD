#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int getNodeLength(char* node_id, int len) {
	int res = 0;
	int i = 0;
	while (i < len && node_id[i] != '_') {
		i++;
	}
	i++;
	while (i < len && node_id[i] != '_') {
		i++;
	}
	i++;
	while (i < len && node_id[i] != '_') {
		i++;
	}
	i++;
	while (i < len && '0' <= node_id[i] && node_id[i] <= '9') {
		res = res * 10 + node_id[i] - '0';
		i++;
	}
	//~ printf("ODE LENGTH PASSE\n");
	return res;
}

int main(int argc, char* argv[]) {
	FILE* f = fopen(argv[1], "r");

	char tok[MAX_LINE];
	char* line;
	char* t;
	int alLen;
	float id;
	int b;
	int e;
	int rb;
	int re;
	char* contig;

	int lmin = atoi(argv[2]);
	int lmax = atoi(argv[3]);
	float idmin = atoi(argv[4]);
	
	fprintf(stderr, "lmin : %d ; lmax : %d\n", lmin, lmax);

	while(fgets(tok, MAX_LINE, f) != NULL) {
		line = malloc((strlen(tok) + 1) * sizeof(char));
		strncpy(line, tok, strlen(tok) + 1);
		t = strtok(tok, "\t");
		t = strtok(NULL, "\t");
		contig = malloc(strlen(t) + 1);
		strncpy(contig, t, strlen(t) + 1);
		//~ fprintf(stderr, "contig : %s\n", contig);
		t = strtok(NULL, "\t");
		id = atof(t);
		t = strtok(NULL, "\t");
		alLen = atoi(t);
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		rb = atoi(t);
		t = strtok(NULL, "\t");
		re = atoi(t);
		t = strtok(NULL, "\t");
		b = atoi(t);
		t = strtok(NULL, "\t");
		e = atoi(t);
		if (id >= idmin && lmin <= alLen && alLen <= lmax && re > rb && e > b) {
		//~ && (b == 1 || e == 1 || b == getNodeLength(contig, strlen(contig)) || e == getNodeLength(contig, strlen(contig)))) {
		//~ && (b == 1 || e == getNodeLength(contig, strlen(contig))) && re > rb) {
			printf("%s", line);
			free(line);
		}
	}
}
