#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define NB_MIN_REPEAT 20000000

// A structure to represent an adjacency list node
typedef struct adj_list_node_s {
    int dest;
	int size1;
	int score1;
    int size2;
    int score2;
    int gap;
    int cov;
    char** LRs;
    struct adj_list_node_s* next;
    int visited;
} *adj_list_node_t;
 
// A structure to represent an adjacency list
typedef struct adj_list_s {
	int len;
	int visited;
	int repeat;
    adj_list_node_t head;
    adj_list_node_t tail;
} *adj_list_t;
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
typedef struct graph_s {
    int size;
    adj_list_t* vertices;
} *graph_t;

typedef struct scaffold_s {
	int beg;
	int end;
	char* seq;
} *scaffold_t;

typedef struct scaffold_tab_s {
	int size;
	scaffold_t* scaffolds;
} *scaffold_tab_t;

char* get_subsequence(char* id, char* file, int pos, int len) {
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		perror("");
		exit(1);
	}
	
	char line[1000000];
	fgets(line, sizeof(line), f);
	int i = 1;
	
	while (line != NULL && strstr(line, id) == NULL) {
		i++;
		fgets(line, sizeof(line), f);
	}
	
	fgets(line, sizeof(line), f);
	fclose(f);
	
	// if len == -1, return the whole sequence
	if (len == -1) {
		len = strlen(line) - 1 - pos;
	}
	
	char* res = malloc(len + 1);
	if (pos + len - 1 >= strlen(line)) {
		fprintf(stderr, "BAH NULL\n");
		return NULL;
	}
	//~ fprintf(stderr, "before cpy : pos = %d; len = %d; strlen = %d\n", pos, len, strlen(line));
	strncpy(res, line + pos, len);
	//~ fprintf(stderr, "after cpy\n");
	res[len] = '\0';
	//~ fprintf(stderr, "returning \n");
	return res;
}

int getNodeNumber(char* node_id, int len) {
	int res = 0;
	int i = 0;
	while (i < len && node_id[i] != 'g') {
		i++;
	}
	i++;
	while (i < len && '0' <= node_id[i] && node_id[i] <= '9') {
		res = res * 10 + node_id[i] - '0';
		i++;
	}
	return res;
}

int getNodeLength(char* node_id, int len) {
	int res = 0;
	int i = 0;
	while (i < len && node_id[i] != '_') {
		i++;
	}
	i++;
	while (i < len && node_id[i] != 'e') {
		i++;
	}
	i++;
	while (i < len && '0' <= node_id[i] && node_id[i] <= '9') {
		res = res * 10 + node_id[i] - '0';
		i++;
	}
	return res;
}
 
// A utility function to create a new adjacency list node
adj_list_node_t newNode(int size1, int score1, char* LR, int gap, int dest, int size2, int score2, int nbLinks) {
    adj_list_node_t node = (adj_list_node_t) malloc(sizeof(*node));
    node->size1 = size1;
    node->score1 = score1;
    node->dest = dest;
    node->size2 = size2;
    node->score2 = score2;
    node->gap = gap;
    node->cov = nbLinks;
    node->next = NULL;
    node->visited = 0;
    node->LRs = malloc(500 * sizeof(char*));
    for (int i = 0; i < 500; i++) {
		node->LRs[i] = NULL;
	}
    if (LR != NULL) {
		node->LRs[0] = malloc(strlen(LR) + 1);
		strncpy(node->LRs[0], LR, strlen(LR) + 1);
	}
    return node;
}

// Creates a graph of size vertices
graph_t createGraph(int size) {
    graph_t graph = (graph_t) malloc(sizeof(*graph));
    graph->size = size;
    
    graph->vertices = (adj_list_t*) malloc(size * sizeof(*(graph->vertices)));
    
    for (int i = 0; i < size; i++) {
        graph->vertices[i] = malloc(sizeof(*(graph->vertices[i])));
        graph->vertices[i]->len = 0;
        graph->vertices[i]->visited = 0;
        graph->vertices[i]->repeat = 0;
        graph->vertices[i]->head = NULL;
        graph->vertices[i]->tail = NULL;
	}
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(graph_t graph, int src, int size1, int score1, char* LR, int gap, int dest, int size2, int score2, int nbLinks) {
    adj_list_node_t node = graph->vertices[src]->head;
    while (node != NULL && node->dest != dest) {
		node = node->next;
	}
	
    if (node != NULL) {
		node->size1 += size1;
		node->score1 += score1;
		node->gap += gap;
		node->size2 += size2;
		node->score2 += score2;
		node->cov += nbLinks;
		if (LR != NULL) {
			node->LRs[node->cov - 1] = malloc(strlen(LR) + 1);
			strncpy(node->LRs[node->cov - 1], LR, strlen(LR) + 1);
		} else {
			node->LRs[node->cov - 1] = NULL;
		}
	} else {
		graph->vertices[src]->repeat++;
		node = newNode(size1, score1, LR, gap, dest, size2, score2, nbLinks);
		if (graph->vertices[src]->head == NULL) {
			graph->vertices[src]->head = node;
		} else {
			graph->vertices[src]->tail->next = node;
		}
		graph->vertices[src]->tail = node;
	}
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph(graph_t graph) {
    int i;
    for (i = 0; i < graph->size; i++) {
        adj_list_node_t node = graph->vertices[i]->head;
		while (node != NULL) {
			if (node->cov > 0) {
				fprintf(stderr, "%d -> %d = %d links and %d gaps and size1 = %d with score %d and size2 = %d with score %d\n", 
						i, node->dest, node->cov, node->gap / node->cov, node->size1 / node->cov, node->score1 / node->cov, node->size2 / node->cov,
						node->score2 / node->cov);
			}
			node = node->next;
		}
    }
}

int getRC(int contig) {
	if (contig < 10000) {
		return contig + 10000;
	} else {
		return contig - 10000;
	}
}

char* revComp(char* seq, int len) {
	char* res = malloc(len + 1);
	for (int i = 0; i < len; i++) {
		switch(seq[i]) {
			case 'A':
				res[len-i-1] = 'T';
				break;
			case 'C':
				res[len-i-1] = 'G';
				break;
			case 'G':
				res[len-i-1] = 'C';
				break;
			case 'T':
				res[len-i-1] = 'A';
				break;
			default:
				res[len-i-1] = 'N';
				break;
		}
	}
	res[len] = '\0';
	return res;
}

char getMax(int A, int C, int G, int T) {
	int m1, m2, r;
	m1 = A > C ? A : C;
	m2 = G > T ? G : T;
	r = m1 > m2 ? m1 : m2;
	if (r == A) return 'A';
	if (r == C) return 'C';
	if (r == G) return 'G';
	return 'T';
}

char* getConsensus(char** sequences, int size, int gapsize) {	
	if (size < 300000) {
		char* res = malloc(strlen(sequences[0]) + 1);
		strncpy(res, sequences[0], strlen(sequences[0]));
		res[strlen(sequences[0])] = '\0';
		return res;
	}
	
	char* res = malloc(gapsize + 1);
	
	int A, C, G, T, N;
	char next;
	for (int i = 0; i < gapsize; i++) {
		A = C = G = T = N = 0;
		for (int j = 0; j < size; j++) {
			if (i < strlen(sequences[j])) {
				switch (sequences[j][i]) {
					case 'A':
						A++;
						break;
					case 'C':
						C++;
						break;
					case 'G':
						G++;
						break;
					case 'T':
						T++;
						break;
					default:
						N++;
						break;
						
				}
			}
		}
		next = getMax(A, C, G, T);
		res[i] = next;
	}
	res[gapsize] = '\0';
	return res;
}

int getLongestContig(graph_t graph) {
	int r = -1;
	
	// Search for the first unvisited contig
	int i = 1;
	while (i < graph->size && r == -1) {
		if (!graph->vertices[i]->visited && graph->vertices[i]->head != NULL) {
			r = i;
		}
		i++;
	}
	
	// Search for the longest unvisited contig
	while (i < graph->size) {
		if (!graph->vertices[i]->visited && graph->vertices[i]->head != NULL && graph->vertices[i]->len > graph->vertices[r]->len) {
			r = i;
		}
		i++;
	}
	
	// If r == -1, no unvisited contig was found
	if (r == -1) return r;
	
	// Check if RC(r) has better quality than r
	int rcr = getRC(r);
	int score1 = 0;
	int score2 = 0;
	
	adj_list_node_t node = graph->vertices[r]->head;
	while (node != NULL) {
		score1 += node->score1;
		node = node->next;
	}
	//~ score1 = score1 / node->cov;
	
	node = graph->vertices[rcr]->head;
	while (node != NULL) {
		score2 += node->score1;
		node = node->next;
	}
	//~ score2 = score2 / node->cov;
	
	return r;
	return score2 < score1 ? r : rcr;
}

adj_list_node_t getLink(graph_t graph, int src, int dest) {
	adj_list_node_t	node = graph->vertices[src]->head;
	while (node != NULL) {
		if (node->dest == dest) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}


void printOrderWithId(graph_t graph, char** contigs, char* c_file, char* r_file) {
	int link;
	int size;
	int scafBeg;
	int cov;
	
	scaffold_tab_t res = malloc(sizeof(*res));
	res->size = 0;
	res->scaffolds = malloc(sizeof(*(res->scaffolds)));
	
	char* seq = NULL;
	char* tmpSeq = NULL;
	int nbScaffs = 0;
	
	int v = getLongestContig(graph);
	scafBeg = v;
	while (v != -1) {
		nbScaffs++;
		adj_list_node_t node = graph->vertices[v]->head;
		
		fprintf(stderr, ">scaffold%d\n", v);
		fprintf(stderr, "%d\n", v);
		if (v < 1000) {
			tmpSeq = get_subsequence(contigs[v], c_file, 0, -1);
		} else {
			tmpSeq = get_subsequence(contigs[v], r_file, 0, -1);
		}
		contigs[v] = NULL;
		contigs[getRC(v)] = NULL;
		seq =  malloc(strlen(tmpSeq) + 1);
		strncpy(seq, tmpSeq, strlen(tmpSeq) + 1);
		//~ free(tmpSeq);
		
		while (node != NULL && !graph->vertices[v]->visited) {
			//~ fprintf(stderr, "On traite %d\n", v);
			// get first unvisited node
			while (node != NULL && ((graph->vertices[node->dest]->visited && graph->vertices[node->dest]->repeat < NB_MIN_REPEAT) || node->cov < 3)) {
				node = node->next;
			}
			
			if (node != NULL) {
				// get best node
				adj_list_node_t t = node;
				size = node->size1 / node->cov + node->size2 / node->cov;
				t = node->next;
				while (t != NULL) {
					if (t->size1 / t->cov + t->size2 / t->cov > size && (!graph->vertices[t->dest]->visited || graph->vertices[t->dest]->repeat >= NB_MIN_REPEAT) && t->cov > 2) {
						link = t->dest;
						size = t->size1 / t->cov + t->size2 / t->cov;
						node = t;
					}
					t = t->next;
				}
			}
			
			// if no unvisited node, get rid of the coverage constraint
			if (node == NULL) {
				node = graph->vertices[v]->head;
				while (node != NULL && graph->vertices[node->dest]->visited && graph->vertices[node->dest]->repeat < NB_MIN_REPEAT) {
					node = node->next;
				}
				
				if (node != NULL) {
					// get best node
					adj_list_node_t t = node;
					size = node->size1 / node->cov + node->size2 / node->cov;
					t = node->next;
					while (t != NULL) {
						if (t->size1 / t->cov + t->size2 / t->cov > size && (!graph->vertices[t->dest]->visited || graph->vertices[t->dest]->repeat >= NB_MIN_REPEAT)) {
							link = t->dest;
							size = t->size1 / t->cov + t->size2 / t->cov;
							node = t;
						}
						t = t->next;
					}
				}
			}
			
			//~ fprintf(stderr, "got next node\n");
			
			if (node != NULL) {
				cov = node->cov;
				link = node->dest;
				
				int start = 0;
				if (node->gap / node->cov > 0) {
					// Consensus des LR
					char** conseq = malloc(node->cov * sizeof(char*));
					int size = 0;
					for (int i = 0; i < node->cov; i++) {
						if (node->LRs[i] != NULL) {
							conseq[size] = node->LRs[i];
							size++;
						}
					}
					tmpSeq = getConsensus(conseq, size, node->gap / node->cov);
					seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
					strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
					free(conseq);
				} else {
					start = - node->gap / node->cov;
				}
				
				if (link < 10000) {
					fprintf(stderr, "f%d\n", link);
					//~ fprintf(stderr, "searching for %s : start = %d\n", contigs[link], start);
					tmpSeq = get_subsequence(contigs[link], c_file, start, -1);
					//~ fprintf(stderr, "got forward sequence\n");
				} else {
					fprintf(stderr, "r%d\n", link);
					//~ fprintf(stderr, "searching for %s : start = %d\n", contigs[link], start);
					tmpSeq = get_subsequence(contigs[link], r_file, start, -1);
					//~ fprintf(stderr, "got reverse sequence\n");
				}
				seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
				strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
				//~ free(tmpSeq);
				graph->vertices[v]->visited = 1;
				graph->vertices[getRC(v)]->visited = 1;
				v = link;
				node = graph->vertices[v]->head;
				//~ fprintf(stderr, "added all n looping\n");
			}
		}
		
		// Try to link scaffolds between them if there's a connexion
		int i = 0;
		int didLink = 0;
		while (i < res->size && didLink == 0) {
			node = getLink(graph, v, res->scaffolds[i]->beg);
			if (node != NULL) {
				int start = 0;
				if (node->gap / node->cov > 0) {
					// Consensus des LR
					char** conseq = malloc(node->cov * sizeof(char*));
					int size = 0;
					for (int i = 0; i < node->cov; i++) {
						if (node->LRs[i] != NULL) {
							conseq[size] = node->LRs[i];
							size++;
						}
					}
					char* tmpSeq = getConsensus(conseq, size, node->gap / node->cov);
					seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
					strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
					free(conseq);
				} else {
					start = - node->gap / node->cov;
				}
				res->scaffolds[i]->beg = scafBeg;
				seq = realloc(seq, strlen(seq) + strlen(res->scaffolds[i]->seq) + 1);
				strncat(seq, res->scaffolds[i]->seq + start, strlen(res->scaffolds[i]->seq) + 1);
				res->scaffolds[i]->seq = seq;
				didLink = 1;
				fprintf(stderr, "LINKED-1 %d to %d\n", v, node->dest);
			} else {
				node = getLink(graph, getRC(scafBeg), res->scaffolds[i]->beg);
				if (node != NULL) {
					char* tmp = revComp(seq, strlen(seq));
					free(seq);
					seq = tmp;
					int start = 0;
					if (node->gap / node->cov > 0) {
						// Consensus des LR
						char** conseq = malloc(node->cov * sizeof(char*));
						int size = 0;
						for (int i = 0; i < node->cov; i++) {
							if (node->LRs[i] != NULL) {
								conseq[size] = node->LRs[i];
								size++;
							}
						}
						char* tmpSeq = getConsensus(conseq, size, node->gap / node->cov);
						seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
						strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
						free(conseq);
					} else {
						start = - node->gap / node->cov;
					}
					res->scaffolds[i]->beg = getRC(v);
					seq = realloc(seq, strlen(seq) + strlen(res->scaffolds[i]->seq) + 1);
					strncat(seq, res->scaffolds[i]->seq + start, strlen(res->scaffolds[i]->seq) + 1);
					res->scaffolds[i]->seq = seq;
					didLink = 1;
					fprintf(stderr, "LINKED-2 %d to %d\n", v, node->dest);
				} else {
					node = getLink(graph, v, getRC(res->scaffolds[i]->end));
					 if (node != NULL) {
						int start = 0;
						if (node->gap / node->cov > 0) {
							// Consensus des LR
							char** conseq = malloc(node->cov * sizeof(char*));
							int size = 0;
							for (int i = 0; i < node->cov; i++) {
								if (node->LRs[i] != NULL) {
									conseq[size] = node->LRs[i];
									size++;
								}
							}
							char* tmpSeq = getConsensus(conseq, size, node->gap / node->cov);
							seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
							strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
							free(conseq);
						} else {
							start = - node->gap / node->cov;
						}
						res->scaffolds[i]->end = getRC(res->scaffolds[i]->end);
						res->scaffolds[i]->beg = scafBeg;
						seq = realloc(seq, strlen(seq) + strlen(res->scaffolds[i]->seq) + 1);
						strncat(seq, revComp(res->scaffolds[i]->seq, strlen(res->scaffolds[i]->seq)) + start, strlen(res->scaffolds[i]->seq) + 1);
						res->scaffolds[i]->seq = seq;
						didLink = 1;
					fprintf(stderr, "LINKED-3 %d to %d\n", v, node->dest);
					} else {
						node = getLink(graph, getRC(scafBeg), getRC(res->scaffolds[i]->end));
						if (node != NULL) {
							char* tmp = revComp(seq, strlen(seq));
							free(seq);
							seq = tmp;
							int start = 0;
							if (node->gap / node->cov > 0) {
								// Consensus des LR
								char** conseq = malloc(node->cov * sizeof(char*));
								int size = 0;
								for (int i = 0; i < node->cov; i++) {
									if (node->LRs[i] != NULL) {
										conseq[size] = node->LRs[i];
										size++;
									}
								}
								char* tmpSeq = getConsensus(conseq, size, node->gap / node->cov);
								seq = realloc(seq, strlen(seq) + strlen(tmpSeq) + 1);
								strncat(seq, tmpSeq, strlen(tmpSeq) + 1);
								free(conseq);
							} else {
								start = - node->gap / node->cov;
							}
							res->scaffolds[i]->end = getRC(res->scaffolds[i]->end);
							res->scaffolds[i]->beg = getRC(v);
							seq = realloc(seq, strlen(seq) + strlen(res->scaffolds[i]->seq) + 1);
							strncat(seq, revComp(res->scaffolds[i]->seq, strlen(res->scaffolds[i]->seq)) + start, strlen(res->scaffolds[i]->seq) + 1);
							res->scaffolds[i]->seq = seq;
							didLink = 1;
						fprintf(stderr, "LINKED-4 %d to %d\n", v, node->dest);
						}
					}
				}
			}
			i++;
		}
		
		if (!didLink) {
			if (nbScaffs > res->size) {
				res->scaffolds = realloc(res->scaffolds, 2 * res->size * sizeof(*(res->scaffolds)) + 1);
			}
			res->size++;
			res->scaffolds[res->size - 1] = malloc(sizeof(*(res->scaffolds[res->size - 1])));
			res->scaffolds[res->size - 1]->beg = scafBeg;
			res->scaffolds[res-> size - 1]->end = v;
			if (scafBeg == v) {
					fprintf(stderr, "ON AJOUTE UN SCAFFOLD UNIQUE\n");
			}
			res->scaffolds[res->size - 1]->seq = malloc(strlen(seq) + 1);
			strncpy(res->scaffolds[res->size - 1]->seq, seq, strlen(seq) + 1);
			free(seq);
		}
		graph->vertices[v]->visited = 1;
		graph->vertices[getRC(v)]->visited = 1;
		v = getLongestContig(graph);
		scafBeg = v;
	}
	
	for (int i = 0; i < res->size; i++) {
		printf(">scaffold%d|size%d\n", i + 1, strlen(res->scaffolds[i]->seq));
		printf("%s\n", res->scaffolds[i]->seq);
		fprintf(stderr, ">scaffold%d|size%d\n", i + 1, strlen(res->scaffolds[i]->seq));
		fprintf(stderr, "%d -> %d\n", res->scaffolds[i]->beg, res->scaffolds[i]->end);
	}
	
	for (int i = 0; i < res->size; i++) {
		free(res->scaffolds[i]->seq);
		free(res->scaffolds[i]);
	}
	free(res);
}

void freeGraph(graph_t graph) {
	for (int i = 0; i < graph->size; i++) {
		adj_list_node_t node = graph->vertices[i]->head;
		adj_list_node_t t;
		while (node != NULL) {
			for (int j = 0; j < 150; j++) {
				free(node->LRs[j]);
			}
			free(node->LRs);
			t = node->next;
			//~ printf("Free node : %d\n", node);
			free(node);
			node = t;
		}
		//~ printf("Free vert : %d\n", graph->vertices[i]);
		free(graph->vertices[i]);
	}
	free(graph->vertices);
	free(graph);
}
 
// Driver program to test above functions
int main(int argc, char* argv[]) {
    // create the graph given in above fugure
    int size = 99999;
    graph_t graph = createGraph(size);
    
    char* contigs[99999];
    for (int i = 0; i < 99999; i++) {
		contigs[i] = NULL;
	}
    
    FILE* f = fopen(argv[1], "r");
	
    char line[1024];
    char* id_src;
    int strand_src;
    int nb_src;
    char* id_dest;
    int strand_dest;
    int nb_dest;
    int nbLinks;
    int size1;
    int score1;
    int size2;
    int score2;
    int gap;
    int pos;
    char* LR;
    char* t;
    char* seq;
	
	fprintf(stderr, "TEST RC : %s\n", revComp("ATAGCTTATGC", 11));
 
	while (fgets(line, 1024, f) != NULL) {
		id_src = strtok(line, "\t");
		nb_src = getNodeNumber(id_src, strlen(id_src));
		strand_src = atoi(strtok(NULL, "\t"));
		nb_src = strand_src == 0 ? nb_src : getRC(nb_src);
		size1 = atoi(strtok(NULL, "\t"));
		score1 = atoi(strtok(NULL, "\t"));
		t = strtok(NULL, "\t");
		LR = malloc(strlen(t) + 1);
		strncpy(LR, t, strlen(t) + 1);
		gap = atoi(strtok(NULL, "\t"));
		pos = atoi(strtok(NULL, "\t"));
		if (gap > 0) {
			seq = get_subsequence(LR, argv[2], pos, gap);
		} else {
			seq = NULL;
		}
		id_dest = strtok(NULL, "\t");
		nb_dest = getNodeNumber(id_dest, strlen(id_dest));
		strand_dest = atoi(strtok(NULL, "\t"));
		nb_dest = strand_dest == 0 ? nb_dest : getRC(nb_dest);
		size2 = atoi(strtok(NULL, "\t"));
		score2 = atoi(strtok(NULL, "\t"));
		nbLinks = atoi(strtok(NULL, "\t"));
		addEdge(graph, nb_src, size1, score1, seq, gap, nb_dest, size2, score2, nbLinks);
		if (seq != NULL) {
			seq = revComp(seq, strlen(seq));
		}
		// AJOUT DU LIEN ENTRE LES RC CAR BLASR NE MAPPE PAR SUR LES RC DES QUERIES
		addEdge(graph, getRC(nb_dest), size2, score2, seq, gap, getRC(nb_src), size1, score1, nbLinks);
		free(seq);
		if (contigs[nb_src] == NULL) {
			contigs[nb_src] = malloc(strlen(id_src) + 1);
			strncpy(contigs[nb_src], id_src, strlen(id_src) + 1);
			graph->vertices[nb_src]->len = getNodeLength(id_src, strlen(id_src));
		}
		nb_src = getRC(nb_src);
		if (contigs[nb_src] == NULL) {
			contigs[nb_src] = malloc(strlen(id_src) + 1);
			strncpy(contigs[nb_src], id_src, strlen(id_src) + 1);
			graph->vertices[nb_src]->len = getNodeLength(id_src, strlen(id_src));
		}
		
		if (contigs[nb_dest] == NULL) {
			contigs[nb_dest] = malloc(strlen(id_dest));
			strncpy(contigs[nb_dest], id_dest, strlen(id_dest) - 1);
			contigs[nb_dest][strlen(id_dest) - 1] = '\0';
			graph->vertices[nb_dest]->len = getNodeLength(id_dest, strlen(id_dest));
		}
		nb_dest = getRC(nb_dest);
		if (contigs[nb_dest] == NULL) {
			contigs[nb_dest] = malloc(strlen(id_dest));
			strncpy(contigs[nb_dest], id_dest, strlen(id_dest) - 1);
			contigs[nb_dest][strlen(id_dest) - 1] = '\0';
			graph->vertices[nb_dest]->len = getNodeLength(id_dest, strlen(id_dest));
		}
	}
	
	printGraph(graph);
	
	fprintf(stderr, "printed graph\n");
	
	printOrderWithId(graph, contigs, argv[3], argv[4]);
	
	fprintf(stderr, "IT'S DONE!!!!! \n");
	
	freeGraph(graph);
	 
    return EXIT_SUCCESS;
}
