#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// A structure to represent an adjacency list node
typedef struct adj_list_node_s {
    int dest;
    int gap;
    int pos;
    char* LR;
    struct adj_list_node_s* next;
    int visited;
} *adj_list_node_t;
 
// A structure to represent an adjacency list
typedef struct adj_list_s {
    adj_list_node_t head;
    adj_list_node_t tail;
} *adj_list_t;
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
typedef struct graph_s {
    int size;
    adj_list_t* vertices;
} *graph_t;

typedef struct scaff_res_s {
	char* scaffold;
	int linked;
} *scaff_res_t;

char* get_sequence(char* id, char* file) {
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		perror("lol");
		exit(1);
	}
	
	char line[1000000];
	fgets(line, sizeof(line), f);
	int i = 1;

	//~ while ((line != NULL && strstr(line, id) == NULL) || 
	while (line != NULL && strstr(line, id) == NULL) {
    //~ ('0' <= line[strlen(id)+1] && line[strlen(id)+1] <= '9')) {
		i++;
		fgets(line, sizeof(line), f);
	}
	
	char* res = fgets(line, sizeof(line), f);
	fclose(f);
	//~ printf("SEQUENCE PASSE\n");
	return res;
}
 
// A utility function to create a new adjacency list node
adj_list_node_t newNode(int dest, int gap, int pos, char* LR) {
    adj_list_node_t node = (adj_list_node_t) malloc(sizeof(*node));
    node->dest = dest;
    node->gap = gap;
    node->pos = pos;
    node->LR = malloc(strlen(LR) + 1);
    strncpy(node->LR, LR, strlen(LR) + 1);
    node->next = NULL;
    node->visited = 0;
    return node;
}

// A utility function that creates a graph of V vertices
graph_t createGraph(int size) {
    graph_t graph = (graph_t) malloc(sizeof(*graph));
    graph->size = size;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->vertices = (adj_list_t*) malloc(size * sizeof(adj_list_t));
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < size; i++) {
        graph->vertices[i] = (adj_list_t) malloc(sizeof(adj_list_t));
        graph->vertices[i]->head = NULL;
        graph->vertices[i]->tail = NULL;
	}
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(graph_t graph, int src, int gap, int pos, char* LR, int dest) {
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    adj_list_node_t node = newNode(dest, gap, pos, LR);
    if (graph->vertices[src]->head == NULL) {
		graph->vertices[src]->head = node;
	} else {
		graph->vertices[src]->tail->next = node;
	}
    graph->vertices[src]->tail = node;
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph(graph_t graph) {
    int i;
    for (i = 0; i < graph->size; i++) {
        adj_list_node_t node = graph->vertices[i]->head;
        if (node != NULL) {
			printf("\n Adjacency list of vertex %d\n : ", i);
			while (node != NULL) {
				printf("%d ", node->dest);
				node = node->next;
			}
			printf("\n");
		}
    }
}

int getNodeNumber(char* node_id, int len) {
	int res = 0;
	int i = 0;
	while (i < len && node_id[i] != '_') {
		i++;
	}
	i++;
	while (i < len && '0' <= node_id[i] && node_id[i] <= '9') {
		res = res * 10 + node_id[i] - '0';
		i++;
	}
	//~ printf("NODE NUMBER PASSE\n");
	return res;
}

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

// A utility function to print the adjacenncy list representation of graph
void traverseGraph(graph_t graph) {
    int i;
    int visited[2000];
    for (i = 0; i < graph->size; i++) {
		for (int j = 0; j < 2000; j++) {
			visited[j] = 0;
		}
		
        adj_list_node_t node = graph->vertices[i]->head;
        if (node != NULL) {
			visited[i] = 1;
			printf("%d ", i);
			while (node != NULL && visited[node->dest] == 0) {
				visited[node->dest] = 1;
				printf("-> %d ", node->dest);
				node = graph->vertices[node->dest]->head;
			}
			printf("\n");
		}
    }
}

int get_distance(char* c1, char* c2, char* file) {
	FILE* f = fopen(file, "r");
	
	//~ fprintf(stderr, "LALA JE VAIS CHERCHER LA DISTANCE\n");
	
	char* src;
	char* dst;
	
	if (getNodeNumber(c1, strlen(c1)) < getNodeNumber(c2, strlen(c2))) {
		src = c1;
		dst = c2;
	} else {
		src = c2;
		dst = c1;
	}
	
	int p1 = 0;
	int p2 = 0;
	
	char* line;
	char tok[1024];
	
	
	
	//~ fprintf(stderr, "JE FAIL A CAUSE DU FGETS\n");
	line = fgets(tok, 1024, f);
		//~ fprintf(stderr, "JE FAIL A CAUSE DU FGETS\n");

	char* t = strtok(tok, "\t");
	while(line != NULL && strcmp(src, t) != 0) {
		//~ printf("|%s| ; |%s|\n", src, t);
		line = fgets(tok, 1024, f);
		t = strtok(tok, "\t");
	}
	
	strtok(NULL, "\t");
	strtok(NULL, "\t");
	p1 = atoi(strtok(NULL, "\t"));
	
	line = fgets(tok, 1024, f);
	t = strtok(tok, "\t");
	while(line != NULL && strcmp(dst, t) != 0) {
		//~ printf("|%s| ; |%s|\n", dst, t);
		line = fgets(tok, 1024, f);
		t = strtok(tok, "\t");
	}
	
	strtok(NULL, "\t");
	strtok(NULL, "\t");
	p2 = atoi(strtok(NULL, "\t"));
	
	fclose(f);
	//~ fprintf(stderr, "dist : %d\n", p1 > p2 ? p1 - p2 : p2 - p1);
	//~ return p1 > p2 ? p1 - p2 : p2 - p1;	
		return p1 > p2 ? p1 - p2 - getNodeLength(dst, strlen(dst)) : p2 - p1 - getNodeLength(src, strlen(src));	

}

// A utility function to print the adjacenncy list representation of graph
void getLongestExtension(graph_t graph, int n, adj_list_node_t node, char** contigs, char* LR_file, char* contigs_file, int* visited, scaff_res_t* scaffolds,
						 char* al_file, int* distance, int* pairs) {
	
	//~ for (int i = 0; i < 99999; i++) {
		//~ visited[i] = 0;
	//~ }
	
	int l1;
	int l2;
	adj_list_node_t node2 = node;
	node = node->next;
	while (node != NULL) {
		l1 = getNodeLength(contigs[node2->dest], strlen(contigs[node2->dest])) + node2->gap;
		l2 = getNodeLength(contigs[node->dest], strlen(contigs[node->dest])) + node->gap;
		if ((l2 > l1 && visited[node->dest] == 0) || (visited[node2->dest] && !visited[node->dest])) {
		//~ if ((node->gap > node2->gap && visited[node->dest] == 0) || (visited[node2->dest] && !visited[node->dest])) {
			node2 = node;
		}
		node = node->next;
	}
	visited[n] = 1;
	int max = getNodeLength(contigs[n], strlen(contigs[n]));
	int size = max;
	
	char* res = malloc(size + 1);
	char* contig = get_sequence(contigs[n], contigs_file);
	strncpy(res, contig, strlen(contig) - 1);
	res[strlen(contig) - 1] = '\0';
	printf("%d ", n);
	while (node2 != NULL && visited[node2->dest] == 0) {
		*pairs = *pairs + 1;
		//~ fprintf(stderr, "MAIS ICI LOL\n");
		*distance = *distance + get_distance(contigs[n], contigs[node2->dest], al_file);
		//~ fprintf(stderr, "HERE\n", res);
		visited[node2->dest] = 1;
		//~ scaffolds[node2->dest]->linked = 1;
		printf("-> %d ", node2->dest);
		int len = getNodeLength(contigs[node2->dest], strlen(contigs[node2->dest]));
		size = size + len + node2->gap;
		max = len > max ? len : max;
		res = (char*) realloc(res, size + 1);
		if (node2->gap > 0) {
			//~ fprintf(stderr, "DUR E TROUVER LR : %s\n", node2->LR);
			char* LR = get_sequence(node2->LR, LR_file);
			strncat(res, LR + node2->pos, node2->gap);
			if (n == 32) {
				//~ fprintf(stderr, "|%d / %d|\n", node2->pos + node2->gap, strlen(LR));
			}
		}
		int overlap = node2->gap < 0 ? - node2->gap : 0; 
		contig = get_sequence(contigs[node2->dest], contigs_file);
		strncat(res, contig + overlap, strlen(contig) - overlap - 1);
		node2 = graph->vertices[node2->dest]->head;
		node = node2;
		if (node != NULL) {
			node = node->next;
			while (node != NULL) {
				l1 = getNodeLength(contigs[node2->dest], strlen(contigs[node2->dest])) + node2->gap;
				l2 = getNodeLength(contigs[node->dest], strlen(contigs[node->dest])) + node->gap;
				if ((l2 > l1 && visited[node->dest] == 0) || (visited[node2->dest] && !visited[node->dest])) {
				//~ if ((node->gap > node2->gap && visited[node->dest] == 0) || (visited[node2->dest] && !visited[node->dest])) {
					node2 = node;
				}
				node = node->next;
			}
		}
	}
	
	//~ printf("TOUJOURS RIEN\n");
	
	//~ fprintf(stderr, "%d\n", strlen(res));
	
	// Le noeud visité n'a pas de successeur ou de successeur raboutable, 
	// mais peut être un successeur d'un autre noeud
	if (max == size && (node2 == NULL || scaffolds[node2->dest]->scaffold == NULL)) {
	//~ if (max == size) {
		visited[n] = 0;
		return;
	}
	
	if (node2 != NULL && scaffolds[node2->dest]->scaffold != NULL) {
		*pairs = *pairs + 1;
		*distance = *distance + get_distance(contigs[n], contigs[node2->dest], al_file);
		printf("-> %d:/ ", node2->dest);
		size = size + node2->gap + strlen(scaffolds[node2->dest]->scaffold);
		res = (char*) realloc(res, size + 1);
		if (node2->gap > 0) {
			char* LR = get_sequence(node2->LR, LR_file);
			strncat(res, LR + node2->pos, node2->gap);
		}
		int overlap = node2->gap < 0 ? - node2->gap : 0;
		contig = scaffolds[node2->dest]->scaffold;
		strncat(res, contig + overlap, strlen(contig) - overlap - 1);
		scaffolds[node2->dest]->linked = 1;
	}
	printf("(max : %d, total : %d)\n", max, size);
	scaffolds[n]->scaffold = malloc(strlen(res) + 1);
	strncpy(scaffolds[n]->scaffold, res, strlen(res) + 1);
	free(res);
	//~ printf("RETOUR FONCTION\n");
	//~ return res;
}

void freeGraph(graph_t graph) {
	for (int i = 0; i < graph->size; i++) {
		adj_list_node_t node = graph->vertices[i]->head;
		adj_list_node_t t;
		while (node != NULL) {
			t = node->next;
			free(node->LR);
			free(node);
			node = t;
		}
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
    
    FILE* f = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    
    
    char* contigs[99999];
    scaff_res_t scaffolds[99999];
    for (int i = 0; i < 99999; i++) {
		contigs[i] = NULL;
		scaffolds[i] = malloc(sizeof(*scaffolds[i]));
		scaffolds[i]->linked = 0;
		scaffolds[i]->scaffold = NULL;
	}
	
	char* line;
    char tok[1024];
    char* id_src;
    int nb_src;
    char* LR;
    int gap;
    int pos;
    char* id_dest;
    int nb_dest;
    char* seq;
    int src_prec;
    int dest_prec;
    char** links;
    int links_size;
    int i_links = 0;
    char* consensus;
    int len;
	
	// V1 : sans consensus
	while (fgets(tok, 1024, f) != NULL) {
		id_src = strtok(tok, "\t");
		nb_src = getNodeNumber(id_src, strlen(id_src));
		LR = strtok(NULL, "\t");
		gap = atoi(strtok(NULL, "\t"));
		pos = atoi(strtok(NULL, "\t"));
		id_dest = strtok(NULL, "\t");
		nb_dest = getNodeNumber(id_dest, strlen(id_dest));
		addEdge(graph, nb_src, gap, pos, LR, nb_dest);
		if (contigs[nb_src] == NULL) {
			contigs[nb_src] = malloc(strlen(id_src) + 1);
			strncpy(contigs[nb_src], id_src, strlen(id_src) + 1);
		}
		
		if (contigs[nb_dest] == NULL) {
			contigs[nb_dest] = malloc(strlen(id_dest));
			strncpy(contigs[nb_dest], id_dest, strlen(id_dest) - 1);
			contigs[nb_dest][strlen(id_dest) - 1] = '\0';		
		}
	}
	
	// V2 : consensus (sans overlap)
	//~ line = fgets(tok, 1024, f);
	//~ if (line != NULL) {
		//~ id_src = strtok(tok, "\t");
		//~ nb_src = getNodeNumber(id_src, strlen(id_src));
		//~ LR = strtok(NULL, "\t");
		//~ gap = atoi(strtok(NULL, "\t"));
		//~ len = gap;
		//~ pos = atoi(strtok(NULL, "\t"));
		//~ id_dest = strtok(NULL, "\t");
		//~ nb_dest = getNodeNumber(id_dest, strlen(id_dest));
		//~ seq = get_subsequence(LR, argv[3], pos, len);
		//~ src_prec = nb_src;
		//~ dest_prec = nb_dest;
		//~ links = malloc(1 * sizeof(char*));
		//~ links_size = 1;
		//~ links[i_links] = seq;
		//~ i_links++;
	//~ }
	
	//~ line = fgets(tok, 1024, f);
	//~ while (line != NULL) {
		//~ printf("LINE : %s\n", line);
		//~ id_src = strtok(tok, "\t");
		//~ nb_src = getNodeNumber(id_src, strlen(id_src));
		//~ LR = strtok(NULL, "\t");
		//~ gap = atoi(strtok(NULL, "\t"));
		//~ pos = atoi(strtok(NULL, "\t"));
		//~ id_dest = strtok(NULL, "\t");
		//~ nb_dest = getNodeNumber(id_dest, strlen(id_dest));
		//~ if (nb_src == src_prec && nb_dest == dest_prec) {
			//~ seq = get_subsequence(LR, argv[3], pos, len);
			//~ if (links_size <= i_links) {
				//~ printf("I REALLOC : %d\n", links_size);
				//~ printf("I GIVE : %d\n", 2 * links_size);
				//~ links = realloc(links, 2 * links_size * sizeof(char*));
				//~ links_size *= 2;
				//~ printf("NOW : %d\n", sizeof(links) / sizeof(char*));
			//~ }
			//~ links[i_links] = seq;
			//~ i_links++;
			//~ line = fgets(tok, 1024, f);
		//~ } else {
			//~ consensus = getConsensus(links, i_links);
			//~ addEdge(graph, src_prec, len, pos, consensus, dest_prec);
			//~ src_prec = nb_src;
			//~ dest_prec = nb_dest;
			//~ for (int i = 0; i < links_size; i++) {
				//~ free(links[i]);
			//~ }
			//~ free(links);
			//~ links = malloc(1 * sizeof(char*));
			//~ links_size = 1;
			//~ i_links = 0;
			//~ seq = get_subsequence(LR, argv[3], pos, gap);
			//~ len = gap;
			//~ links[i_links] = seq;
			//~ line = fgets(tok, 1024, f);
		//~ }
	//~ }
	//~ consensus = getConsensus(links, i_links);
	//~ for (int i = 0; i < links_size; i++) {
		//~ free(links[i]);
	//~ }
	//~ free(links);
	//~ addEdge(graph, src_prec, len, pos, consensus, dest_prec);
	
	//~ printf("I READ IT ALL\n");
	
	//~ return EXIT_SUCCESS;
	
	int visited[graph->size];
	for (int i = 0; i < graph->size; i++) {
		visited[i] = 0;
	}
	
	int distance = 0;
	int pairs = 0;
	for (int i = 0; i < 99999; i++) {
		adj_list_node_t node = graph->vertices[i]->head;
		if (node != NULL && visited[i] == 0) {
		//~ if (node != NULL) {
			printf("Je visite : %d\n", i);
			getLongestExtension(graph, i, node, contigs, argv[3], argv[4], visited, scaffolds, argv[5], &distance, &pairs);
			//~ printf("LALA : %s\n", res);
			//~ fprintf(out, ">SCAFFOLD_%d.\n", i),
			//~ fprintf(out, "%s\n", res);
			//~ free(res);
		}
	}
	
	fprintf(stderr, "DISTANCE MOYENNE ENTRE DEUX SCAFFOLDS LIES : %d\n", distance / pairs);
	
	for (int i = 0; i < 99999; i++) {
		if (scaffolds[i]->scaffold != NULL && scaffolds[i]->linked == 0) {
			fprintf(out, ">SCAFFOLD_%d_length_%d\n", i, strlen(scaffolds[i]->scaffold)),
			fprintf(out, "%s\n", scaffolds[i]->scaffold);
		} else if (visited[i] == 0 && contigs[i] != NULL) {
			//~ fprintf(out, ">SCAFFOLD_%d_length_%d\n", i, getNodeLength(contigs[i], strlen(contigs[i])));
			//~ fprintf(out, "%s", get_sequence(contigs[i], argv[4]));
		}
	}
	
	for (int i = 0; i < 99999; i++) {
		free(contigs[i]);
		free(scaffolds[i]);
	}
	
	freeGraph(graph);
	 
    return EXIT_SUCCESS;
}
