#ifndef BSTREE_H
#define BSTREE_H

#include "invertedIndex.h"

struct ListNode {
    char *value;
	struct ListNode *next;
};
typedef struct ListNode *List;

//Functions for manipulating the Inverted Index BST
//Creates a new tree
InvertedIndexBST newTree(void);
//Creates a new tree node
InvertedIndexBST newNode(char *word, int d, char *file);
//Inserts a node into a tree based on alphabetical order
InvertedIndexBST InvertedIndexBSTInsert (InvertedIndexBST t, char *word, int d, char *file);
//Finds a node in the tree
InvertedIndexBST FindNode(InvertedIndexBST t, char *word);
//Prints a tree in Infix order
void InvertedIndexBSTreeInfix (InvertedIndexBST t, FILE *fp);
//Prints a tree node
void InvertedIndexshowBSTreeNode (InvertedIndexBST t, FILE *fp);

//Functions for manipulating the FileList data structure
//Creates a new file list node
FileList newFileList(int d, char *file);
//Inserts a new file into the file list based on alphabetical order
FileList FileListInsert(FileList l, int d, char *file);
//Creates a copy of a file list node
FileList MakeListNodeCopy(FileList src);
//Returns the length of a file list
int LengthFileList(FileList l);
//Prints a file list
void showFileList(FileList l, FILE *fp);

//Functions for manipulating the TfIdf List data structure
//Calculates a TfIdf Value
double calculateTfIdfVal(double tf, int c, int D);
//Creates a new TfIdf list
TfIdfList newTfIdfList(void);
//Creates a new list node
TfIdfList newTfIdfNode(double tfIdf, char *file);
//Inserts a new node into the list based on descending tfidf value order
TfIdfList TfIdfInsert(TfIdfList l, double tfIdf, char *file);
//makes a copy of a TfIdf node
TfIdfList MakeTfIdfNodeCopy(TfIdfList src);
//Checks if a file is already in the list
int inTfIdfList(TfIdfList l, char *value);
//Finds a node in a list
TfIdfList findNodeTfIdfList(TfIdfList l, char *value);
//Reorders a TfIdfList into descending order
TfIdfList reorderTfIdfList(TfIdfList src, TfIdfList dest);
//Frees all memory asociated with a TfIdfList
void freeTfIdfList(TfIdfList l);

//Functions for manipulating my List data structure (defined at the top of the file)
//Creates a new list
List newList(void);
//Creates a new list node
List newListNode(char *value);
//Appends an item to the end of the list
List appendItemList(List l, char *value);
//Frees all memory associated with the list
void freeList(List l);

#endif
