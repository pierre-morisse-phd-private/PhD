#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// A structure to represent an adjacency list node
typedef struct adj_list_node_s {
    int dest;
    int cov;
    int alLen;
    float score;
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
	//~ printf("NODE NUMBER PASSE\n");
	return res;
}
 
// A utility function to create a new adjacency list node
adj_list_node_t newNode(int dest, int nbLinks, int alLen, float score) {
    adj_list_node_t node = (adj_list_node_t) malloc(sizeof(*node));
    node->dest = dest;
    node->cov = nbLinks;
    node->alLen = alLen;
    node->score = score;
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
void addEdge(graph_t graph, int src, int dest, int nbLinks, int alLen, float score) {
    adj_list_node_t node = graph->vertices[src]->head;
    
    while (node != NULL && node->dest != dest) {
		node = node->next;
	}
	
    if (node != NULL) {
		node->cov += nbLinks;
		node->alLen += alLen;
		node->score += score;
	} else {
		node = newNode(dest, nbLinks, alLen, score);
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
        if (node != NULL) {
			printf("%d -> ", i);
			while (node != NULL) {
				printf("%d (%d - %d - %f) ; ", node->dest, node->cov, node->alLen / node->cov, node->score / node->cov);
				node = node->next;
			}
			printf("\n");
		}
    }
}

int getRC(int contig) {
	if (contig < 1000) {
		return contig + 1000;
	} else {
		return contig - 1000;
	}
}

void printOrderAllLinksWithId(graph_t graph) {
	//~ res_list_t res = malloc(sizeof(*res));
	int cov;
	int link;
	int placed[graph->size];
	int v;
	float id;
	
	for (int i = 0; i < graph->size; i++) {
		placed[i] = 0;
	}
	
	for (int i = 0; i < graph->size; i++) {
		v = i;
		adj_list_node_t node = graph->vertices[v]->head;
		
		while (node != NULL) {
		//~ while (node != NULL) {
			//~ printf("Visite de %d\n", v);
			cov = node->cov;
			id = node->score;
			link = node->dest;
			//~ node = node->next;
			//~ // Get max weight edge
			//~ while (node != NULL) {
				//~ if (node->id > id && node->cov > cov && node->dest != getRC(v)) {
					//~ cov = node->cov;
					//~ link = node->dest;
					//~ id = node->id;
				//~ }
				//~ node = node->next;
			//~ }
			
			//~ int addLink = (cov > 1 && !placed[link] && link != getRC(i));
			int addLink = (link != getRC(v) && cov > 0 && !placed[getRC(v)] && !placed[getRC(link)]) ? 1 : 0;
			
			
			
			// Check if i -> link is of stricly greater weight than RC(i) −> link and RC(i) -> RC(link)
			adj_list_node_t t = graph->vertices[getRC(i)]->head;
			while (t != NULL && addLink) {
				if ((t->dest == link || t->dest == getRC(link)) && t->score / t->cov >= id) {
					addLink = 0;
				}
				t = t->next;
			}
			
			// Check if i -> link is of strictly greater weight than link -> i and link -> RC(i)
			t = graph->vertices[link]->head;
			while (t != NULL && addLink) {
				if ((t->dest == v || t->dest == getRC(v)) && t->score / t->cov >= id) {
				//~ if (t->dest == i && t->cov >= cov) {
					addLink = 0;
				}
				t = t->next;
			}
			
			//~ printf("link -> dest\n");
			
			// Check if i -> link if of strictly greater weight than RC(link) -> i and RC(link) -> RC(i)
			//~ printf("%d ; RC : %d\n", link, getRC(link));
			t = graph->vertices[getRC(link)]->head;
			while (t != NULL && addLink) {
				if ((t->dest == v || t->dest == getRC(v)) && t->score / t->cov >= id) {
				//~ if (t->dest == i && t->cov >= cov) {
					addLink = 0;
				}
				t = t->next;
			}
			
			// Add i -> link to list it has the best weight
			if (addLink) {
				printf("%d\t%d\t%d\n", v, link, cov);
				placed[v] = 1;
				placed[link] = 1;
				//~ v = link;
				//~ node = graph->vertices[v]->head;
			}
			
			node = node->next;
			
			//~ printf("Visite de %d finie\n", i);
		}
	}
	//~ printf("Fonction finie\n");
}


void printOrderWithId(graph_t graph) {
	//~ res_list_t res = malloc(sizeof(*res));
	int cov;
	int link;
	float id;
	int placed[graph->size];
	int v;
	int alLen;
	
	for (int i = 0; i < graph->size; i++) {
		placed[i] = 0;
	}
	
	for (int i = 0; i < graph->size; i++) {
	//~ for (int i = graph->size - 1; i > 0; i--) {
		v = i;
		
		adj_list_node_t node = graph->vertices[v]->head;
		if (placed[v]) {
			node = NULL;
		}
		
		while (node != NULL) {
		//~ while (node != NULL) {
			//~ printf("Visite de %d\n", v);
			cov = node->cov;
			link = node->dest;
			id = node->score / cov;
			alLen = node->alLen / cov;
			//~ id = node->id / cov;
			node = node->next;
			//~ // Get max weight edge
			while (node != NULL) {
				if (node->score / node->cov > id && node->dest != getRC(v) && !placed[node->dest] && !placed[getRC(node->dest)]) {
					cov = node->cov;
					link = node->dest;
					id = node->score / cov;
					alLen = node->alLen / cov;
				}
				node = node->next;
			}
			
			//~ printf("lien vers : %d\n", link);
			
			//~ int addLink = (cov > 1 && !placed[link] && link != getRC(i));
			int addLink = (link != getRC(v) && cov > 0 && !placed[getRC(v)] && !placed[getRC(link)]) ? 1 : 0;
			
			//~ printf("addLink : %d\n", addLink);
			
			// Check if i -> link is of stricly greater weight than RC(i) −> link and RC(i) -> RC(link)
			
			adj_list_node_t t = graph->vertices[getRC(i)]->head;
			if (!placed[v]) {
				while (t != NULL && addLink) {
					if ((t->dest == link || t->dest == getRC(link)) && t->score / t->cov >= id) {
						addLink = 0;
					}
					t = t->next;
				}
			}
			
			//~ printf("addLink : %d\n", addLink);
			
			// Check if i -> link is of strictly greater weight than link -> i
			t = graph->vertices[link]->head;
			while (t != NULL && addLink) {
				if ((t->dest == v) && t->score / t->cov >= id) {
				//~ if (t->dest == i && t->cov >= cov) {
					addLink = 0;
				}
				t = t->next;
			}
			
			// Check if i -> link is of strictly greater weight than link -> RC(i)
			if (!placed[v]) {
				t = graph->vertices[link]->head;
				while (t != NULL && addLink) {
					if ((t->dest == getRC(v)) && t->score / t->cov >= id) {
					//~ if (t->dest == i && t->cov >= cov) {
						addLink = 0;
					}
					t = t->next;
				}
			}
			
			//~ printf("addLink : %d\n", addLink);
			
			//~ printf("link -> dest\n");
			
			// Check if i -> link if of strictly greater weight than RC(link) -> i
			//~ printf("%d ; RC : %d\n", link, getRC(link));
			t = graph->vertices[getRC(link)]->head;
			while (t != NULL && addLink) {
				if ((t->dest == v) && t->score / t->cov >= id) {
				//~ if (t->dest == i && t->cov >= cov) {
					addLink = 0;
				}
				t = t->next;
			}
			
			// Check if i -> link if of strictly greater weight than RC(link) -> RC(i)
			//~ printf("%d ; RC : %d\n", link, getRC(link));
			if (!placed[v]) {
				t = graph->vertices[getRC(link)]->head;
				while (t != NULL && addLink) {
					if ((t->dest == getRC(v)) && t->score / t->cov >= id) {
					//~ if (t->dest == i && t->cov >= cov) {
						addLink = 0;
					}
					t = t->next;
				}
			}
			
			//~ printf("addLink : %d\n", addLink);
			
			// Add i -> link to list it has the best weight
			if (addLink) {
				printf("%d\t%d\t%d\t%d\t%f\n", v, link, cov, alLen, id);
				placed[v] = 1;
				placed[link] = 1;
				v = link;
				node = graph->vertices[v]->head;
			} else {
				node = NULL;
			}
			
			//~ printf("Visite de %d finie\n", i);
		}
	}
	//~ printf("Fonction finie\n");
}

void freeGraph(graph_t graph) {
	for (int i = 0; i < graph->size; i++) {
		adj_list_node_t node = graph->vertices[i]->head;
		adj_list_node_t t;
		while (node != NULL) {
			t = node->next;
			free(node);
			node = t;
		}
		free(graph->vertices[i]);
	}
	free(graph->vertices);
	free(graph);
}

void countHits(graph_t graph) {
	int* res = malloc(graph->size * sizeof(int));
	for (int i = 0; i < graph->size; i++) {
		res[i] = 0;
	}
	
	for (int i = 0; i < graph->size; i++) {
		adj_list_node_t node = graph->vertices[i]->head;
		while (node != NULL) {
			res[i] += node->cov;
			res[node->dest] += node->cov;
			node = node->next;
		}
	}
	
	for (int i = 0; i < graph->size; i++) {
		if (res[i] != 0) {
			printf("%d : %d\n", i, res[i]);
		}
	}
}
 
// Driver program to test above functions
int main(int argc, char* argv[]) {
    // create the graph given in above fugure
    int size = 99999;
    graph_t graph = createGraph(size);
    
    FILE* f = fopen(argv[1], "r");
	
    char line[1024];
    char* id_src;
    int strand_src;
    int nb_src;
    char* id_dest;
    int strand_dest;
    int nb_dest;
    float score;
    int nbLinks;
    int alLen;
 
	while (fgets(line, 1024, f) != NULL) {
		id_src = strtok(line, "\t");
		nb_src = getNodeNumber(id_src, strlen(id_src));
		strand_src = atoi(strtok(NULL, "\t"));
		nb_src = strand_src == 0 ? nb_src : getRC(nb_src);
		strtok(NULL, "\t");
		(strtok(NULL, "\t"));
		(strtok(NULL, "\t"));
		id_dest = strtok(NULL, "\t");
		nb_dest = getNodeNumber(id_dest, strlen(id_dest));
		strand_dest = atoi(strtok(NULL, "\t"));
		nb_dest = strand_dest == 0 ? nb_dest : getRC(nb_dest);
		nbLinks = atoi(strtok(NULL, "\t"));
		alLen = atoi(strtok(NULL, "\t"));
		score = atoi(strtok(NULL, "\t"));
		addEdge(graph, nb_src, nb_dest, nbLinks, alLen, score);
	}
	
	printGraph(graph);

	//~ countHits(graph);
	
	printf("\n\n");
	
	
	printOrderWithId(graph);
	
	//~ printf("FINI\n");
	
	freeGraph(graph);
	 
    return EXIT_SUCCESS;
}
