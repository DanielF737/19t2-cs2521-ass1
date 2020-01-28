#ifndef SEARCHPAGERANK_H
#define SEARCHPAGERANK_H

#include "myADTs.h"

Tree BuildTree(void);
PageList BuildPageList(char **names, int length);
CountedPageList retrieve(Tree tree, PageList list, char *searchWords[], char **names, int Length, int numWords);

#endif
