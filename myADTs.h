#ifndef BSTREE_H
#define BSTREE_H

#include "graph.h"

struct ListNode {
    char *value;
	struct ListNode *next;
};
typedef struct ListNode *List;

struct PageListNode {
    int url;
    int degrees;
	double value;
	struct PageListNode *next;
};
typedef struct PageListNode *PageList;

struct CountedPageListNode {
    int url;
    int occur;
    int degrees;
	double value;
	struct CountedPageListNode *next;
};
typedef struct CountedPageListNode *CountedPageList;

struct TreeNode {
    char *word;
    List fileList;
    struct TreeNode *left;
    struct TreeNode *right;
};
typedef struct TreeNode *Tree;

//My implementation of strdup
char *dupestring(char *original);

//When given an array finds the corresponding id
int findID(char **names, char *name, int length);

//Makes a string lowercase
char * lowerCase(char *string);

//Functions for manipulating my List data structure (defined at the top of the file)
//Creates a new list
List newList(void);
//Creates a new list node
List newListNode(char *value);
//Appends an item to the end of the list
List appendItemList(List l, char *value);
//Returns the length of a List
int lengthList(List l);
//prints the list to STDOUT
void showList(List l);
//Frees all memory associated with the list
void freeList(List l);

//Functions for manipulating my PageList data structure (defined at the top of the file)
//Creates a new Pagelist
PageList newPageList(void);
//Creates a new Pagelist node
PageList newPageListNode(int url, int degrees, double value);
//Appends an item to the end of the Pagelist
PageList appendItemPageList(PageList l, int url, int degrees, double value);
//Sorts a page list
PageList sortPageList(PageList src, PageList dest);
//Returns a page list node when searching by ID
PageList findPageList(PageList l, int url);
//Returns 0 or 1 whether or not a url is in a PageList respectively
int inPageList(PageList l, int url);
//Returns the length of a PageList
int lengthPageList(PageList l);
//Prints a pagelist to a specified file pointer
void showPageList(PageList l, char **names, FILE *outfile);
//Frees all memory associated with the Pagelist
void freePageList(PageList l);

//Functions for manipulating my CountedPageList data structure (defined at the top of the file)
//Creates a new CountedPagelist
CountedPageList newCountedPageList(void);
//Creates a new CountedPagelist node
CountedPageList newCountedPageListNode(int url, int degrees, double value);
//Returns a counted page list node when searching by ID
CountedPageList findCountedPageList(CountedPageList l, int url);
//Appends an item to the end of the CountedPagelist
CountedPageList appendItemCountedPageList(CountedPageList l, int url, int degrees, double value, int occur);
//Sorts a counted page list
CountedPageList sortCountedPageList(CountedPageList src, CountedPageList dest);
//Returns 0 or 1 whether or not a url is in a CountedPageList respectively
int inCountedPageList(CountedPageList l, int url);
//Frees all memory associated with the CountedPagelist
void freeCountedPageList(CountedPageList l);

//My functions for working with the provided graph datastructure
//Counts inbound connections to a graph node
int inConnect(Graph g, Vertex dest);
//Counts outgoing connections to a graph node
int outConnect(Graph g, Vertex src);
//Calculates Win value
double Win(Graph g, int v, int u);
//Calculates Wout value
double Wout(Graph g,int v, int u);

//Functions for working with my Tree ADT
//Returns a NULL pointer as a new Tree
Tree newTree(void);
//Creates a new empty tree node
Tree newTreeNode(char *word, List files);
//Function to insert an item into the tree using the AVL method
//Adapted from 2019 lab04
Tree insertAVL(Tree t, char* it, List files);
//Function to rotate Tree Right
//Adapted from 2019 lab04
Tree rotateL (Tree n2);
//Function to rotate Tree Right
//Adapted from 2019 lab04
Tree rotateR (Tree n1);
//Calculates depth beneath current node
//Adapted from 2019 lab04
int depth (Tree t);
//Searches the tree to find a node
Tree FindNode(Tree t, char *word);
//Print the tree in Linear order
void ShowTreeLinear (Tree t);
//Print the tree in Infix order
void ShowTreeInfix (Tree t);
//Prints a tree node
void ShowTreeNode (Tree t);
//Frees memory associated with tree
void dropTree(Tree t);

#endif
