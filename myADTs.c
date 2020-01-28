// BSTree.c ... implementation of binary search tree ADT

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>

#include "myADTs.h"
#include "invertedIndex.h"

//Returns a null pointer to act as an empty tree node
InvertedIndexBST newTree(void) {
    return NULL;
}

//Allocates nescessary memory and assigns values for new tree nodes
InvertedIndexBST newNode(char *word, int d, char *file) {
    InvertedIndexBST new = malloc (sizeof *new);
    new->word = malloc(sizeof(char)*(strlen(word)+1));
    strcpy(new->word, word);
    new->fileList= newFileList(d, file);
    new->left = new->right = NULL;
    return new;
}

//Inserts a node into the tree based on alphabetical order, or if the word is already in the
//tree, add the file to the file list
InvertedIndexBST InvertedIndexBSTInsert (InvertedIndexBST t, char *word, int d, char *file) {
    if (t == NULL) 
		return newNode(word, d, file);
	else if (strcmp(t->word, word) < 0) 
		t->left = InvertedIndexBSTInsert (t->left, word, d, file);
    else if (strcmp(t->word, word) > 0) 
		t->right = InvertedIndexBSTInsert (t->right, word, d, file);
    if (strcmp(t->word, word) == 0) 
        t->fileList=FileListInsert(t->fileList, d, file);
    return t;
}

//Navigates the tree to find a word, if a word doesnt exist in the tree, return a NULL pointer
InvertedIndexBST FindNode(InvertedIndexBST t, char *word) {
    if (t==NULL) 
        return NULL;
    else if (strcmp(word, t->word) > 0) 
        return FindNode(t->left, word);
    else if (strcmp(word, t->word) < 0) 
        return FindNode(t->right, word);
    else 
        return t;
}

//Print the tree in Infix order
void InvertedIndexBSTreeInfix (InvertedIndexBST t, FILE *fp) {
	if (t == NULL) return;
	InvertedIndexBSTreeInfix (t->right, fp);
	InvertedIndexshowBSTreeNode (t, fp);
	InvertedIndexBSTreeInfix (t->left, fp);
}

//Prints a tree node
void InvertedIndexshowBSTreeNode (InvertedIndexBST t, FILE *fp) {
	if (t == NULL) return;
	fprintf (fp, "%s ", t->word);
    showFileList(t->fileList, fp);
    fprintf(fp, "\n");
}

//Initializes a node for a file list, allocating necessary memory and assigning values
FileList newFileList(int d, char *file) {
    FileList newF = malloc(sizeof(FileList));
    newF->filename = malloc(sizeof(char)*(strlen(file)+1));
    strcpy(newF->filename,file);
    newF->tf=(1/(double)d);
    newF->next = NULL;
    return newF;
}

//Insert a node into a file list based on alphabetical order, if the file exists in the 
//list, alter the Tf value aproprietly
FileList FileListInsert(FileList l, int d, char *file) {
    if (l==NULL)
        return newFileList(d, file);
    else if (strcmp(file, l->filename)<0) {
        FileList new = MakeListNodeCopy(l);
        l->filename=realloc(l->filename, sizeof(char)*(strlen(file)+1));
        strcpy(l->filename,file);
        l->next=new;
        l->tf=1/(double)d;
    } else if (strcmp(file, l->filename)==0)
        l->tf=((l->tf*(double)d)+1)/(double)d; //tf*d = the number of times the word already occured in the file
    else
        l->next=FileListInsert(l->next,d,file);
    return l;
}

//Makes a copy of a list node
FileList MakeListNodeCopy(FileList src) {
    FileList new = newFileList(1, src->filename);
    new->tf=src->tf;
    new->next=src->next;
    return new;
}

//Recursively finds the length of a file list
int LengthFileList(FileList l) {
    int count;
    if (l==NULL)
        return 0;
    else 
        return count = 1+LengthFileList(l->next);
}

//Prints a file list node
void showFileList(FileList l, FILE *fp) {
    if (l==NULL) return;
    fprintf(fp, "%s ", l->filename);
    showFileList(l->next, fp);
    return;
}

//Calculates a TfIdf value based on tf, the number of files containing the word (c) and the total number of files (D)
double calculateTfIdfVal(double tf, int c, int D) {
    double tfIdf=(-1)*tf* log10((double) c/(double) D);
    return tfIdf;
}

//Returns a null pointer to act as an empty list node
TfIdfList newTfIdfList(void) {
    return NULL;
}

//Allocates nescessary memory and assigns values for new list nodes
TfIdfList newTfIdfNode(double tfIdf, char *file) {
    TfIdfList new = malloc(sizeof(*new));
    new->filename = malloc(sizeof(char)*(strlen(file)+1));
    strcpy(new->filename,file);
    new->tfidf_sum = tfIdf;
    new->next=NULL;
    return new;
}

//Inserts a node into the list based on descending tfidf order. If they have the same tfidf value, sort alphabetically
TfIdfList TfIdfInsert(TfIdfList l, double tfIdf, char *file){
    if (l==NULL)
        return newTfIdfNode(tfIdf, file);
    else if (tfIdf > l->tfidf_sum) {
        TfIdfList new = MakeTfIdfNodeCopy(l);
        l->filename=realloc(l->filename, sizeof(char)*(strlen(file)+1));
        strcpy(l->filename,file);
        l->tfidf_sum=tfIdf;
        l->next=new;
        return l;
    } else if (tfIdf < l->tfidf_sum)
        l->next=TfIdfInsert(l->next, tfIdf, file);
    else {
        if (strcmp(file, l->filename) < 0) {
            TfIdfList new = MakeTfIdfNodeCopy(l);
            l->filename=realloc(l->filename, sizeof(char)*(strlen(file)+1));
            strcpy(l->filename,file);
            l->tfidf_sum=tfIdf;
            l->next=new;
            return l;
        } else
            l->next=TfIdfInsert(l->next, tfIdf, file);
    }
    return l;
}

//Makes a copy of a list node
TfIdfList MakeTfIdfNodeCopy(TfIdfList src) {
    TfIdfList new = newTfIdfNode(src->tfidf_sum, src->filename);
    new->next=src->next;
    return new;
}

//Returns a 1 or a 0 depending on whether a file is in or not in a tfidf list respectively
int inTfIdfList(TfIdfList l, char *value) {
    if (l==NULL)
        return 0;
    else if (strcmp(l->filename, value)==0)
        return 1;
    else
        return inTfIdfList(l->next, value);
}

//Find a node in a TfIdf list and return it
TfIdfList findNodeTfIdfList(TfIdfList l, char *value) {
    if (l==NULL)
        return NULL;
    else if (strcmp(l->filename, value)==0)
        return l;
    else
        return findNodeTfIdfList(l->next, value);
}

//Sorts a tfidf list into descending order (alphabetical if the tfidf values are the same) and stores them in a 
//new list
TfIdfList reorderTfIdfList(TfIdfList src, TfIdfList dest){
    if (src==NULL) 
        return NULL;
    else {
        dest=TfIdfInsert(dest, src->tfidf_sum, src->filename);
        reorderTfIdfList(src->next, dest);
        return dest;
    }
}

//Free all memory associated with a tfidf list
void freeTfIdfList(TfIdfList l) {
    if (l->next!=NULL) freeTfIdfList(l->next);
    free(l->filename);
    free(l);
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

//free all memory associated with a list
void freeList(List l) {
    if (l->next!=NULL)
        freeList(l->next);
    free(l->value);
    free(l);
    return;
}