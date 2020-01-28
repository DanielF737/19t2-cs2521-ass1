#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "pagerank.h"
#include "myADTs.h"
#include "graph.h"
#include "item.h"
#include "Queue.h"

int main(int argc, char  *argv[]) {
    if (argc != 4) { //Ensure correct inputs
        printf("Usage = ./pagerank d  diffPR  maxIterations\n");
        return -1;
    }

    //Read files from collection.txt into a linked list
    List files = NULL;
    files = ReadCollectionfromFile("collection.txt");
    //Populate the names array with the names of the files corresponding to a numerical id
    //for use with the adjacency matrix graph structure
    int numWords=lengthList(files);
    char *names[numWords];
    List trav = files;
    int i = 0;
    while (trav!=NULL) {
        names[i] = dupestring(trav->value);
        i++;
        trav=trav->next;
    }

    //Create a graph of the urls from the files in the list
    Graph graph = GraphUrls(files, names);

    //Find the pageranks of all the urls in the graph
    PageList list= newPageList();
    list = pageRank(graph, strtod(argv[1],NULL), strtod(argv[2],NULL), atoi(argv[3]));

    //Sort the list of pageranks by pagerank value
    PageList sorted = newPageList(); 
    sorted=sortPageList(list, sorted);
    freePageList(list);

    //Print the sorted page rank list to pagerankList.txt
    FILE *outfile=fopen("pagerankList.txt", "w");
    showPageList(sorted, names, outfile);
    fclose(outfile);

    //Free memory
    freePageList(sorted);
    dropGraph(graph);

    return 0;
}


List ReadCollectionfromFile(char *collectionFile){
    FILE *collection = fopen(collectionFile, "r"); //Open the collection files
    char line[1000]; //initialise array to read lines into
    List filesList=NULL; //Initialize list that will hold all the files to read

    //read through the collections file, append them to the file list
    while (fgets(line,1000,collection)!=NULL) {
        char *f;
        f = strtok(line," \n");
        while (f != NULL && strcmp(f,"\n")!=0) {
            filesList=appendItemList(filesList, f);
            f = strtok(NULL, " \n");
        }
    }

    fclose(collection);
    return filesList;
}

List ReadCollectionfromArray(char **line) {
    List filesList=NULL; //Initialize list that will hold all the files to read
    int i=0;
    //read through the array, append files to the file list
    while (line[i]!=NULL) {
        char *f;
        f = strtok(line[i]," \n");
        while (f != NULL && strcmp(f,"\n")!=0) {
            filesList=appendItemList(filesList, f);
            f = strtok(NULL, " \n");
        }
        i++;
    }

    return filesList;
}

Graph GraphUrls(List l, char **names) {
    Graph g = newGraph(lengthList(l));
    List root=l; //store the list of the root to free later
    int currentNode=0;
    //Loop through the list
    while (l!=NULL) {

        FILE *header = fopen(strcat(l->value, ".txt"), "r");
        int i=0;
        char line[1000]; //initialise array to read lines into
        char *links[1000];
        //Initialise first 2 values
        links[0]=dupestring("");
        links[1]=dupestring("");
        fgets(line,1000,header); //Skip the first line that states start of section 1

        //read till the end of section 1, store the names of outbound links in the array
        while (fgets(line,1000,header)!=NULL && strcmp(line, "#end Section-1\n")!=0) { 
            links[i]=dupestring(line);
            i++; 
        }
        fclose(header);
        
        //Create a list of files from the names in the array
        List outgoing = ReadCollectionfromArray(links);
        List traveler=outgoing; //Create a variable to naviagate the list without losing the root

        //For each node in the list, add an edge to the graph between the current file
        //and the file its linking to
        while (traveler!=NULL) {
            int toNode = FindUrl(traveler->value, names, lengthList(root));
            if (toNode >= 0 && currentNode!=toNode)
                insertEdge(g, currentNode, toNode, 1);
            traveler=traveler->next;
        }
        
        //Free allocated memory
        if (outgoing!=NULL)
            freeList(outgoing);
        currentNode++;
        l=l->next;
    }
    freeList(root);
    return g;
}

//Search the names array for a url, if found return the numeric id
int FindUrl(char *url, char **names, int max) {
	int i=0;
	for (i = 0; i < max; i++) {
		if (strcmp (names[i], url) == 0)
			return i;
	}
	return -1;
}


PageList pageRank(Graph g, double d, double diffPR, int maxIterations){
    PageList pagelist = newPageList();
    int N=g->nV;
    double ranks[N][maxIterations]; //Create an array to store the pageranks data
    
    //Populate the array with the initial values
    for (int i =0; i < N; i++)
        ranks[i][0] = (double)1/N;

    //Complete the pagerank calculations
    int iteration = 0;
    double diff = diffPR;
    while (iteration < maxIterations && diff >= diffPR){  
        for (int i = 0; i < N; i++) { //for each node;
            double sum = 0;
            for (int j=0; j <N; j++){
                if (isConnected(g, j, i) && j != i)
                sum += ranks[j][iteration] * Win(g, j, i) * Wout(g,j, i);
            }
            ranks[i][iteration +1] = (double)((1-d)/N) + d*sum;
        }
        double sum2 =0;
        for (int i =0; i < N; i++){
            sum2 += fabs(ranks[i][iteration + 1] - ranks[i][iteration]);
        }
        diff = sum2;
        iteration++;
    }
    
    //Create a pageList from the final pagerankings calculations
    for (int i = 0; i < N; i ++)
            pagelist = appendItemPageList(pagelist, i, outConnect(g, i), ranks[i][iteration]); 

    return pagelist;
}