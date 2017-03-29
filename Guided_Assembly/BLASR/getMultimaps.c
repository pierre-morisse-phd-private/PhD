#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char* line1 = NULL;
	char* line2 = NULL;
	char tok[1024];
	char* t;
	char* lr1 = NULL;
	char* lr2 = NULL;
	char* c1 = NULL;
	char* c2 = NULL;
	
	FILE* in = fopen(argv[1], "r");
	FILE* out = fopen(argv[2], "a");
	
	if (fgets(tok, 1024, in) != NULL) {
		line1 = malloc(strlen(tok) + 1);
		strncpy(line1, tok, strlen(tok) + 1);
		t = strtok(tok, "\t");
		lr1 = malloc(strlen(t) + 1);
		strncpy(lr1, t, strlen(t) + 1);
		t = strtok(NULL, "\t");
		c1 = malloc(strlen(t) + 1);
		strncpy(c1, t, strlen(t) + 1);
	}
	
	while (line1 != NULL) {		
		free(line2);
		line2 = NULL;
		if (fgets(tok, 1024, in) != NULL) {
			line2 = malloc(strlen(tok) + 1);
			strncpy(line2, tok, strlen(tok) + 1);
			t = strtok(tok, "\t");
			free(lr2);
			lr2 = malloc(strlen(t) + 1);
			strncpy(lr2, t, strlen(t) + 1);
			t = strtok(NULL, "\t");
			free(c2);
			c2 = malloc(strlen(t) + 1);
			strncpy(c2, t, strlen(t) + 1);
			
			//~ if (strcmp(lr1, lr2) == 0 && strcmp(c1, c2) != 0) {
			if (strcmp(lr1, lr2) == 0) {
				fprintf(out, "%s", line1);
			}
		}
		
		while (line2 != NULL && strcmp(lr1, lr2) == 0) {
			//~ if (strcmp(c1, c2) != 0) {
				fprintf(out, "%s", line2);
			//~ }
			//~ printf("LALA : %s ; %s\n", lr1, lr2);
			free(line2);
			line2 = NULL;
			if (fgets(tok, 1024, in) != NULL) {
				line2 = malloc(strlen(tok) + 1);
				strncpy(line2, tok, strlen(tok) + 1);
				t = strtok(tok, "\t");
				free(lr2);
				lr2 = malloc(strlen(t) + 1);
				strncpy(lr2, t, strlen(t) + 1);
				free(c1);
				c1 = malloc(strlen(c2) + 1);
				strncpy(c1, c2, strlen(c2) + 1);
				free(c2);
				t = strtok(NULL, "\t");
				c2 = malloc(strlen(t) + 1);
				strncpy(c2, t, strlen(t) + 1);
			}
		}
		free(line1);
		line1 = NULL;
		if (line2 != NULL) {
			line1 = malloc(strlen(line2) + 1);
			strncpy(line1, line2, strlen(line2) + 1);
		}
		free(lr1);
		lr1 = malloc(strlen(lr2) + 1);
		strncpy(lr1, lr2, strlen(lr2) + 1);
		free(c1);
		c1 = malloc(strlen(c2) + 1);
		strncpy(c1, c2, strlen(c2) + 1);
	}
	
	free(line1);
	free(line2);
	free(lr1);
	free(lr2);
	
	return EXIT_SUCCESS;
}
