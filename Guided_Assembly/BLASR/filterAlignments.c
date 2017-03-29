#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2048

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
	int tStart;
	int tEnd;
	int tLen;
	int alLen;
	float id;
	int score;
	char* contig;

	int lmin = atoi(argv[2]);
	int lmax = atoi(argv[3]);
	float idmin = atoi(argv[4]);
	
	fprintf(stderr, "lmin : %d ; lmax : %d\n", lmin, lmax);

	while(fgets(tok, MAX_LINE, f) != NULL) {
		//~ fprintf(stderr, "LOL\n");
		line = malloc((strlen(tok) + 1) * sizeof(char));
		strncpy(line, tok, strlen(tok) + 1);
		// qName
		t = strtok(tok, " ");
		// tName
		t = strtok(NULL, " ");
		contig = malloc(strlen(t) + 1);
		strncpy(contig, t, strlen(t) + 1);
		// qStrand
		t = strtok(NULL, " ");
		// tStrand
		t = strtok(NULL, " ");
		// score
		t = strtok(NULL, " ");
		score = atoi(t);
		// percentSimilarity
		t = strtok(NULL, " ");
		id = atof(t);
		// tStart
		t = strtok(NULL, " ");
		tStart = atoi(t);
		// tEnd
		t = strtok(NULL, " ");
		tEnd = atoi(t);
		alLen = tEnd - tStart;
		// tLength
		t = strtok(NULL, " ");
		tLen = atoi(t);
		// qStart
		t = strtok(NULL, " ");
		// qEnd
		t = strtok(NULL, " ");
		// qLen
		t = strtok(NULL, " ");
		// nCels
		t = strtok(NULL, " ");
		if (id >= idmin && lmin <= alLen && alLen <= lmax ){
			//~ && (tStart <= 100 || tLen - tEnd <= 100)) {
		//~ && (b == 1 || e == 1 || b == getNodeLength(contig, strlen(contig)) || e == getNodeLength(contig, strlen(contig)))) {
		//~ && (b == 1 || e == getNodeLength(contig, strlen(contig))) && re > rb) {
			printf("%s", line);
			free(line);
		}
	}
}
