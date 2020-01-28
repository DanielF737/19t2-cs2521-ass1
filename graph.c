//Adapted for assignment

// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "Queue.h"

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
/*static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}*/

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0/* && g->edges[w][v] != 0*/)
		// an edge already exists; do nothing.
		return;
	g->edges[v][w] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);
	
	int visited[g->nV];
	int i, j;
	Vertex v = 0;
	Queue route = newQueue();
	Queue q = newQueue();
	
	for (i = 0; i < g->nV; i++){
        visited[i] = -1;
    }

    visited[src] = src;
    QueueJoin(q, src);
    
    while (!QueueIsEmpty(q)){
        v = QueueLeave(q);
        
        if (v == dest)
            break;
        
        for (j = 0; j < g->nV; j++){
            //if edge exists and its not visited and within max range
            if (g->edges[v][j] > 0 && visited[j] == -1 && g->edges[v][j] <= max){
                //add it to the ToDo Queue
                visited[j] = v;
                QueueJoin(q,j);       
        	}
        }	
	}
	
	if (visited[dest] == -1){
	    //no result
	    dropQueue(route);
	    dropQueue(q);
	    return 0;
	}
	
	QueueJoin (route, v);
	int index = 0;
	
	while (v != src){
	    QueueJoin(route, visited[v]);
	    index++;
	    v = visited[v];
	}
	
	for (i = index; i >= 0; i--){
	    path[i] = QueueLeave(route);
	}
	
	dropQueue(route);
	dropQueue(q);
	return index + 1;
}

int countEdges(Graph g, Vertex start) {
	int total=0;
	for (int i=0; i < g->nV; i++) {
		if (g->edges[start][total] != 0)
			total++;
	}
	return total;
}

int isConnected(Graph g, Vertex i, Vertex j) {
	if (g->edges[i][j]) 
		return 1;
	return 0;
}
