#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNodeLength(char* node_id, int len) {
	//~ printf("LEL : %s\n", node_id);
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
	//~ printf("RES : %d\n", res);
	return res;
}

int main(int argc, char* argv[]) {
	char* line = NULL;
	char tok[1024];
	char* t;
	
	char* q1id = NULL;
	char* q2id = NULL;
	char* r1id = NULL;
	char* r2id = NULL;
	int r1len;
	int r2len;
	int eq1;
	int er1;
	int sq2;
	int sr2;
	int q11, q12, r11, r12, q21, q22, r21, r22;
	int r1strand;
	int r2strand;
	int gap;
	float id;
	int alLen;
	int nbLinks;
	float outId;
	int score;
	int tStart;
	int tEnd;
	int size1;
	int score1;
	int size2;
	int score2;
	
	FILE* in = fopen(argv[1], "r");
	int minGap = atoi(argv[2]);
	int maxGap = atoi(argv[3]);
	int maxOv = atoi(argv[4]);
	int minExt = atoi(argv[5]);
	FILE* out = fopen(argv[6], "a");

	line = fgets(tok, 1024, in);
	if (line != NULL) {
		// qName
		t = strtok(tok, " ");
		q1id = malloc(strlen(t) + 1);
		strncpy(q1id, t, strlen(t) + 1);
	}
	
	while (line != NULL) {
		// Read a line
		// tName
		t = strtok(NULL, " ");
		free(r1id);
		r1id = malloc(strlen(t) + 1);
		strncpy(r1id, t, strlen(t) + 1);
		// qStrand
		t = strtok(NULL, " ");
		// tStrand
		t = strtok(NULL, " ");
		r1strand = atoi(t);
		// Score
		t = strtok(NULL, " ");
		score1 = atoi(t);
		// percentSimilarity
		t = strtok(NULL, " ");
		id = atoi(t);
		// tStart
		r11 = atoi(strtok(NULL, " "));
		// tEnd
		r12 = atoi(strtok(NULL, " "));
		size1 = r12 - r11;
		// tLen
		r1len = atoi(strtok(NULL, " "));
		// qStart
		q11 = atoi(strtok(NULL, " "));
		// qEnd
		q12 = atoi(strtok(NULL, " "));
		// qLength
		t = strtok(NULL, " ");
		// nCells
		t = strtok(NULL, " ");
		eq1 = q11 > q12 ? q11 : q12;
		er1 = r11 > r12 ? r11 : r12;
		nbLinks = 0;
		
		// Reads a new line to compare LR with the first one
		line = fgets(tok, 1024, in);
		if (line != NULL) {
				// qName
				t = strtok(tok, " ");
				free(q2id);
				q2id = malloc(strlen(t) + 1);
				strncpy(q2id, t, strlen(t) + 1);
		}
		
		// Keep reading line while LR is the same to get (Contig, LR, contig) triplets
		while (line != NULL && strcmp(q1id, q2id) == 0) {
			// tName
			t = strtok(NULL, " ");
			free(r2id);
			r2id = malloc(strlen(t) + 1);
			strncpy(r2id, t, strlen(t) + 1);
			// qStrand
			t = strtok(NULL, " ");
			// tStrand
			t = strtok(NULL, " ");
			r2strand = atoi(t);
			// score
			t = strtok(NULL, " ");
			score2 = atoi(t);
			// percentSimilarity
			t = strtok(NULL, " ");
			id = atof(t);
			// tStart
			r21 = atoi(strtok(NULL, " "));
			// tEnd
			r22 = atoi(strtok(NULL, " "));
			size2 = r22 - r21;
			// tLen
			r2len = atoi(strtok(NULL, " "));
			// qStart
			q21 = atoi(strtok(NULL, " "));
			// qEnd
			q22 = atoi(strtok(NULL, " "));
			// qLength
			t = strtok(NULL, " ");
			// nCells
			t = strtok(NULL, " ");
			sq2 = q22 > q21 ? q21 : q22;
			sr2 = r22 > r21 ? r21 : r22;
			nbLinks++;
			if (strcmp(r1id, r2id) != 0) {
				//~ printf("%d - %d - %d - %d + %d\n", sq2, eq1, sr2, r1len, er1);
				gap = sq2 - eq1 - sr2 - r1len + er1;
				//~ printf("%d\n", gap);
				if ((gap >= minGap && gap <= maxGap) || (gap <= 0 && gap >= maxOv && gap + r2len >= minExt)) {
				//~ if (1) {
					int pos = eq1 + r1len - er1;
					//~ fprintf(out, "%s\t%s\t%d\t%d\t%s\t%f\n", r1id, q1id, gap, pos, r2id, id / 100 * alLen);
					fprintf(out, "%s\t%d\t%d\t%d\t%s\t%d\t%d\t%s\t%d\t%d\t%d\t%d\n",
						r1id, r1strand, size1, score1, q2id, gap, pos, r2id, r2strand, size2, score2, 1);
					printf("links : %d\n", nbLinks);
					//~ fprintf(out, "%s\n", line);
				}
				nbLinks = 0;
				outId = 0;
			} else {
				size1 = size2;
				score1 = score2;
				//~ nbLinks++;
			}
			free(r1id);
			r1id = malloc(strlen(r2id) + 1);
			strncpy(r1id, r2id, strlen(r2id) + 1);
			r1len = r2len;
			r1strand = r2strand;
			eq1 = q21 > q22 ? q21 : q22;
			er1 = r21 > r22 ? r21 : r22;
			line = fgets(tok, 1024, in);
			if (line != NULL) {
				t = strtok(tok, " ");
				free(q2id);
				q2id = malloc(strlen(t) + 1);
				strncpy(q2id, t, strlen(t) + 1);
			}
		}
		free(q1id);
		q1id = malloc(strlen(q2id) + 1);
		strncpy(q1id, q2id, strlen(q2id) + 1);
	}
	
	free(q1id);
	free(r1id);
	free(q2id);
	free(r2id);
	
	fclose(in);
	fclose(out);
	
	return EXIT_SUCCESS;
}
