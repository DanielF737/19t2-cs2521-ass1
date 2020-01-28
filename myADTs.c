// BSTree.c ... implementation of binary search tree ADT

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>

#include "myADTs.h"

//My implementation of strdup
char *dupestring(char *original) {
    int len;
    static char *dupe;
    char *offset;

    //Allocate memory
    len = strlen(original);
    dupe = (char *)malloc(sizeof(char)*len+1);
    if (dupe == NULL)
        return( (char *)NULL);

    //Copy 
    offset = dupe;
    while(*original) {
        *offset = *original;
        offset++;
        original++;
    }
    *offset = '\0';

    return(dupe);
}

//When given an array finds the corresponding id
int findID(char **names, char *name, int length) {
    int i=0;
    while (i<length) {
        if (strcmp(names[i],name)==0)
            return i;
        i++;
    }
    return -1;
}

//Makes a string lowercase
char * lowerCase(char *string) {
    int length = strlen(string), i=0;
    while (i<length) {
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i]+=32;
        i++;
    }
    return string;
}

//Returns a null pointer to act as an empty list node
List newList(void) {
    return NULL;
}

//Allocates nescessary memory and assigns values for new list nodes
List newListNode(char *value) {
    List new = malloc(sizeof(*new));
    new->value= malloc(sizeof(char)*(strlen(value)+1));
    strcpy(new->value, value);
    new->next=NULL;
    return new;
}

//Appends an item to the end of a list
List appendItemList(List l, char *value) {
    if (l==NULL) 
        return newListNode(value);
    else 
        l->next=appendItemList(l->next, value);
    return l;
}

//Returns the length of a List
int lengthList(List l){
    if (l!=NULL)
        return 1+lengthList(l->next);
    return 0;
}

//prints the list to STDOUT
void showList(List l){
    if (l==NULL)
        return;
    printf("%s ", l->value);
    showList(l->next);
    return;
}

//free all memory associated with a list
void freeList(List l) {
    if (l->next!=NULL)
        freeList(l->next);
    free(l->value);
    free(l);
    return;
}

//Creates a new Pagelist
PageList newPageList(void){
    return NULL;
}

//Creates a new Pagelist node
PageList newPageListNode(int url, int degrees, double value) {
    PageList new = malloc(sizeof(*new));
    new->url = url;
    new->degrees=degrees;
    new->value=value;
    new->next=NULL;
    return new;
}

//Appends an item to the end of the Pagelist
PageList appendItemPageList(PageList l, int url, int degrees, double value) {
    if (l==NULL)
        return newPageListNode(url, degrees, value);
    else { 
        if (value>l->value) {
            PageList new = newPageListNode(l->url, l->degrees, l->value);
            new->next = l->next;
            l->next = new;
            l->url = url;
            l->degrees=degrees;
            l->value=value;
        } else 
            l->next=appendItemPageList(l->next, url, degrees, value);
    }
    return l;
}

//Sorts a page list
PageList sortPageList(PageList src, PageList dest){
    if (src==NULL) 
        return NULL;
    else {
        dest=appendItemPageList(dest, src->url, src->degrees, src->value);
        sortPageList(src->next, dest);
        return dest;
    }
}

//Returns a page list node when searching by ID
PageList findPageList(PageList l, int url) {
    if (l==NULL) 
        return NULL;
    if (l->url==url) 
        return l;
    return findPageList(l->next, url);
}

//Returns 0 or 1 whether or not a url is in a PageList respectively
int inPageList(PageList l, int url) {
    if (l==NULL)
        return 1;
    if (l->url==url)
        return 0;
    return inPageList(l->next, url);
}

//Returns the length of a PageList
int lengthPageList(PageList l) {
    if (l!=NULL)
        return 1+lengthPageList(l->next);
    return 0;
}

//Prints a pagelist to a specified file pointer
void showPageList(PageList l, char **names, FILE *outfile) {
    if (l!=NULL) {
        fprintf(outfile, "%s, %d, %0.7f\n", names[l->url], l->degrees, l->value);
        showPageList(l->next, names, outfile);
    }
    return;
}

//Frees all memory associated with the Pagelist
void freePageList(PageList l) {
    if (l==NULL) return;
    if (l->next!=NULL)
        freePageList(l->next);
    free(l);
    return;
}

//Creates a new CountedPagelist
CountedPageList newCountedPageList(void) {
    return NULL;
}

//Creates a new CountedPagelist node
CountedPageList newCountedPageListNode(int url, int degrees, double value) {
    CountedPageList new = malloc(sizeof(*new));
    new->url = url;
    new->occur=0;
    new->degrees=degrees;
    new->value=value;
    new->next=NULL;
    return new;
}

//Returns a counted page list node when searching by ID
CountedPageList findCountedPageList(CountedPageList l, int url) {
    if (l==NULL) 
        return NULL;
    if (l->url==url) 
        return l;
    return findCountedPageList(l->next, url);
}

//Appends an item to the end of the CountedPagelist
CountedPageList appendItemCountedPageList(CountedPageList l, int url, int degrees, double value, int occur) {
    if (l==NULL)
        return newCountedPageListNode(url, degrees, value);
    else { 
        if (occur == l->occur) {
            if (value>l->value) {
                CountedPageList new = newCountedPageListNode(l->url, l->degrees, l->value);
                new->next = l->next;
                l->next = new;
                l->url = url;
                l->degrees=degrees;
                l->value=value;
                l->occur=occur;
            } else 
                l->next=appendItemCountedPageList(l->next, url, degrees, value, occur);
        } else if (occur>l->occur) {
            CountedPageList new = newCountedPageListNode(l->url, l->degrees, l->value);
            new->next = l->next;
            l->next = new;
            l->url = url;
            l->degrees=degrees;
            l->value=value;
            l->occur=occur;
        } else 
            l->next=appendItemCountedPageList(l->next, url, degrees, value, occur);
    }
    return l;
}


//Sorts a counted page list
CountedPageList sortCountedPageList(CountedPageList src, CountedPageList dest) {
    if (src==NULL) 
        return NULL;
    else {
        dest=appendItemCountedPageList(dest, src->url, src->degrees, src->value, src->occur);
        sortCountedPageList(src->next, dest);
        return dest;
    }
}

//Returns 0 or 1 whether or not a url is in a CountedPageList respectively
int inCountedPageList(CountedPageList l, int url){
    if (l==NULL)
        return 1;
    if (l->url==url)
        return 0;
    return inCountedPageList(l->next, url);
}

//Frees all memory associated with the CountedPagelist
void freeCountedPageList(CountedPageList l) {
    if (l==NULL) return;
    if (l->next!=NULL)
        freeCountedPageList(l->next);
    free(l);
    return;
}

//Counts inbound connections to a graph node
int inConnect(Graph g, Vertex dest) {
	int connections = 0;
	for (int i = 0; i<g->nV; i++) {
		if (g->edges[i][dest])
			connections++;
	}
	return connections;
}

//Counts outgoing connections to a graph node
int outConnect(Graph g, Vertex src) {
	int connections = 0;
	for (int i = 0; i<g->nV; i++) {
		if (g->edges[src][i])
			connections++;
	}
	return connections;
}

//Calculates Win value
double Win(Graph g, int v, int u){
    double numer = inConnect(g,u)==0? 0.5:inConnect(g,u);
    double denom=0;
    for (int i=0; i < g->nV; i++){
        if (i==v)
            continue;
        if (g->edges[v][i]){
            if (inConnect(g,i) == 0)
                denom += 0.5;
            else
                denom += inConnect(g,i);
        }
    }
    if (denom==0)
        denom=0.5;
    return numer/denom;
}

//Calculates Wout value
double Wout(Graph g, int v, int u){
    double numer = outConnect(g,u)==0? 0.5:outConnect(g,u);
    double denom=0;
    for (int i=0; i < g->nV; i++){
        if (i==v)
            continue;
        if (g->edges[v][i]) {
            if (outConnect(g,i) == 0)
                denom += 0.5;
            else
                denom += outConnect(g,i);
        }
    }
    if (denom==0)
        denom=0.5;
    return numer/denom;
}

//Returns a NULL pointer as a new Tree
Tree newTree(void) {
    return NULL;
}
//Creates a new empty tree node
Tree newTreeNode(char *word, List files) {
    Tree new = malloc(sizeof(*new));
    new->word = malloc(sizeof(char)*(strlen(word)+1));
    strcpy(new->word, word);
    new->fileList=files;
    new->left=NULL;
    new->right=NULL;
    return new;
}

//Function to insert an item into the tree using the AVL method
//Adapted from 2019 lab04
Tree insertAVL(Tree t, char* it, List files) {
	if (t == NULL) return newTreeNode (it, files);
	int diff = strcmp(it, t->word);
	if (diff == 0) {
        t->word = it;
        t->fileList = files;
    } else if (diff > 0) t->left  = insertAVL (t->left, it, files);
	else if (diff < 0) t->right = insertAVL (t->right, it, files);

	int dL = depth(t->left);
	int dR = depth(t->right);
	if ((dL - dR) > 1) t = rotateR (t);
	if ((dR - dL) > 1) t = rotateL (t);
	return t;
}

//Function to rotate Tree Right
//Adapted from 2019 lab04
Tree rotateL (Tree n2)
{
	if (n2 == NULL) return NULL;
	Tree n1 = n2->right;
	if (n1 == NULL) return n2;
	n2->right = n1->left;
	n1->left = n2;
	return n1;
}

//Function to rotate Tree Right
//Adapted from 2019 lab04
Tree rotateR (Tree n1) {
	if (n1 == NULL) return NULL;
	Tree n2 = n1->left;
	if (n2 == NULL) return n1;
	n1->left = n2->right;
	n2->right = n1;
	return n2;
}

//Calculates depth beneath current node
//Adapted from 2019 lab04
int depth (Tree t) {
	if (t == NULL) return 0;
	int ldepth = depth (t->left);
	int rdepth = depth (t->right);
	return 1 + ((ldepth > rdepth) ? ldepth : rdepth);
}

//Searches the tree to find a node
Tree FindNode(Tree t, char *word) {
    if (t==NULL) 
        return NULL;
    else if (strcmp(word, t->word) > 0) 
        return FindNode(t->left, word);
    else if (strcmp(word, t->word) < 0) 
        return FindNode(t->right, word);
    else 
        return t;
}

//Print the tree in Linear order
void ShowTreeLinear (Tree t) {
	if (t == NULL) return;
	ShowTreeNode (t);
	ShowTreeLinear (t->right);
	ShowTreeLinear (t->left);
}

//Print the tree in Infix order
void ShowTreeInfix (Tree t) {
	if (t == NULL) return;
	ShowTreeInfix (t->right);
	ShowTreeNode (t);
	ShowTreeInfix (t->left);
}

//Prints a tree node
void ShowTreeNode (Tree t) {
	if (t == NULL) return;
	printf ("%s ", t->word);
    showList(t->fileList);
    printf("\n");
}

//Frees memory associated with tree
void dropTree(Tree t) {
    if (t==NULL)
        return;
    dropTree(t->left);
    dropTree(t->right);
    freeList(t->fileList);
    free(t->word);
    free(t);
    return;
}