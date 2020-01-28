#ifndef PAGERANK_H
#define PAGERANK_H

#include "myADTs.h"
#include "graph.h"
#include "item.h"
#include "Queue.h"

List ReadCollectionfromFile(char *collectionFile);
List ReadCollectionfromArray(char **line);
Graph GraphUrls(List l, char **names);

int FindUrl(char *url, char **names, int max);

PageList pageRank(Graph g, double d, double diffPR, int maxIterations);
#endif
