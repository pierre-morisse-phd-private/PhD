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
	int gap;
	
	FILE* in = fopen(argv[1], "r");
	int minGap = atoi(argv[2]);
	int maxGap = atoi(argv[3]);
	int maxOv = atoi(argv[4]);
	int minExt = atoi(argv[5]);
	FILE* out = fopen(argv[6], "a");

	line = fgets(tok, 1024, in);
	if (line != NULL) {
		t = strtok(tok, "\t");
		q1id = malloc(strlen(t) + 1);
		strncpy(q1id, t, strlen(t) + 1);
	}
	
	while (line != NULL) {
		// Read a line
		t = strtok(NULL, "\t");
		free(r1id);
		r1id = malloc(strlen(t) + 1);
		strncpy(r1id, t, strlen(t) + 1);
		r1len = getNodeLength(r1id, strlen(r1id));
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		t = strtok(NULL, "\t");
		q11 = atoi(strtok(NULL, "\t"));
		q12 = atoi(strtok(NULL, "\t"));
		r11 = atoi(strtok(NULL, "\t"));
		r12 = atoi(strtok(NULL, "\t"));
		eq1 = q11 > q12 ? q11 : q12;
		er1 = r11 > r12 ? r11 : r12;
		
		// Keep reading lines while LR is the same to get (LR, contig) couples
		line = fgets(tok, 1024, in);
		if (line != NULL) {
				t = strtok(tok, "\t");
				free(q2id);
				q2id = malloc(strlen(t) + 1);
				strncpy(q2id, t, strlen(t) + 1);
		}
		
		while (line != NULL && strcmp(q1id, q2id) == 0) {
			t = strtok(NULL, "\t");
			free(r2id);
			r2id = malloc(strlen(t) + 1);
			strncpy(r2id, t, strlen(t) + 1);
			r2len = getNodeLength(r2id, strlen(r2id));
			t = strtok(NULL, "\t");
			t = strtok(NULL, "\t");
			t = strtok(NULL, "\t");
			t = strtok(NULL, "\t");
			q21 = atoi(strtok(NULL, "\t"));
			q22 = atoi(strtok(NULL, "\t"));
			r21 = atoi(strtok(NULL, "\t"));
			r22 = atoi(strtok(NULL, "\t"));
			sq2 = q22 > q21 ? q21 : q22;
			sr2 = r22 > r21 ? r21 : r22;
			if (strcmp(r1id, r2id) != 0) {
				printf("%d - %d - %d - %d + %d\n", sq2, eq1, sr2, r1len, er1);
				gap = sq2 - eq1 - sr2 - r1len + er1;
				//~ printf("%d\n", gap);
				if ((gap >= minGap && gap <= maxGap) || (gap <= 0 && gap >= maxOv && gap + r2len >= minExt)) {
					int pos = eq1 + r1len - er1;
					fprintf(out, "%s\t%s\t%d\t%d\t%s\n", r1id, q1id, gap, pos, r2id);
				}
			}
			free(r1id);
			r1id = malloc(strlen(r2id) + 1);
			strncpy(r1id, r2id, strlen(r2id) + 1);
			r1len = r2len;
			eq1 = q21 > q22 ? q21 : q22;
			er1 = r21 > r22 ? r21 : r22;
			line = fgets(tok, 1024, in);
			if (line != NULL) {
				t = strtok(tok, "\t");
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
	
	return EXIT_SUCCESS;
}
