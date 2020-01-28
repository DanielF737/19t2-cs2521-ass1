#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>

#include "searchPagerank.h"
#include "myADTs.h"

int main (int argc, char  *argv[]) {
    Tree words=BuildTree(); //Read words into a tree

    //Count the length of the pagerank file to determine how many words
    FILE *infile = fopen("pagerankList.txt", "r");
    int length = 0;
    char line[1000];
    while (fgets(line,1000,infile)!=NULL) {
        length++;
    }

    //Read the names of the files into an array
    fseek(infile, 0, SEEK_SET);
    char *names[length];
    int i = 0;
    while (fgets(line,1000,infile)!=NULL) {
        char *f;
        f = strtok(line,", \n");
        names[i] = dupestring(f);
        i++;
    }

    fclose(infile);
    //Build a pagelist based on the WPR data from pagerankList.txt
    PageList list= BuildPageList(names, length);

    //Get search words from ARGV
    char *searchWords[argc-1];
    i = 1;
    int j=0;
    while (i<argc) {
        searchWords[j] = dupestring(lowerCase(argv[i]));
        i++;j++;
    }
    //Retrieve the pagelist of the pages containing the search words in order of WPR
    CountedPageList ordered = retrieve(words, list, searchWords, names, length, argc-1);
    CountedPageList trav = ordered;
    
    //Travel the list and print the URLs in order
    while (trav!=NULL) {
        printf("%s\n", names[trav->url]);
        trav=trav->next;
    }

    //Free all used memory
    freePageList(list);
    freeCountedPageList(ordered);
    dropTree(words);
    return 0;
}

Tree BuildTree(void) {
    //Open the invertedIndex.txt file
    FILE *infile=fopen("invertedIndex.txt", "r");
    char line[1000];
    Tree words = newTree();

    //Read the word into the tree node, then all the files into that nodes file list
    while (fgets(line,1000,infile)!=NULL) {
        char *word;
        char *f;
        word = strtok(line," \n");
        List files=newList();
        f = strtok(NULL," \n");
        while (f != NULL && strcmp(f,"\n")!=0) {
            files=appendItemList(files, f);
            f = strtok(NULL, " \n");
        }
        //Use the AVL insert method so that the tree isnt just a list as the file is 
        //Already sorted
        words=insertAVL(words, lowerCase(word), files);
    }

    fclose(infile);
    return words;
}

PageList BuildPageList(char **names, int length) {
    //Open the pagerank file
    PageList list = newPageList();
    FILE *infile=fopen("pagerankList.txt", "r");
    char line[1000];

    //Read through the file and insert each files data into a pagerank list
    while (fgets(line,1000,infile)!=NULL) {
        char *url;
        int degree;
        double pagerank;
        url = strtok(line,", \n");
        int urlID= findID(names, url, length);

        degree = atoi(strtok(NULL,", \n"));
        pagerank = strtod(strtok(NULL,", \n"),NULL);

        list=appendItemPageList(list, urlID, degree, pagerank);
    }

    fclose(infile);

    return list;
}

CountedPageList retrieve(Tree tree, PageList list, char *searchWords[], char **names, int Length, int numWords) {
    int i = 0;
    CountedPageList root = newCountedPageList();
    list->url=0;

    //While there are still words to search
    while (i<numWords) {
        Tree word = FindNode(tree, searchWords[i]);
        if (word==NULL) { //If we somehow read an empty list entry, move on in the loop
            i++;
            continue;
        }
        List l = word->fileList;
        while (l!=NULL) {
            //If the file is already in the counted list, increment the counter,
            //Else add it in the list
            if (inCountedPageList(root, findID(names, l->value, Length))==1) {
                PageList unConFound = findPageList(list, findID(names, l->value, Length));
                CountedPageList found = newCountedPageListNode(unConFound->url, unConFound->degrees, unConFound->value);
                found->occur++;
                root=appendItemCountedPageList(root, found->url, found->degrees, found->value, 0);
            } else {
                CountedPageList found = findCountedPageList(root, findID(names, l->value, Length));
                found->occur++;
            }
            l=l->next;
        }
        i++;
    }
    
    //Resort the pagelist by occurance order
    CountedPageList ordered = newCountedPageList();
    ordered = sortCountedPageList(root, ordered);
    free(root);
    return ordered;
}