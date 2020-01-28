#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>


#include "invertedIndex.h"
#include "myADTs.h"



char * normaliseWord(char *str) {
    int end = strlen(str);
    int start = 0;

    //Cut leading and trailing whitespace
    while (str[start]==' ') start++;
    while (str[end]==' ') end--;
    end--;
    
    //Check to see if last character is punctuation and move it
    if (str[end] == '.'||str[end] == ','||str[end] == ';'||str[end] == '?') end--;
    
    //copy characters to correct positions in string
    int i, j=0;
    for (i=start;i<=end;i++){
        if (str[i] >= 'A' && str[i] <= 'Z') str[j]=str[i]+32;
        else     str[j]=str[i];
        j++;
    }
    //add a null terminator to the end of the string
    str[j]='\0';

    return str;
}

InvertedIndexBST generateInvertedIndex(char *collectionFile) {
    FILE *collection = fopen(collectionFile, "r"); //Open the collection files
    char line[1000]; //initialise array to read lines into
    List filesList=newList(); //Initialize list that will hold all the files to read

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
    InvertedIndexBST root = newTree();//initialize a new Inverted Index tree    
    List curr = filesList; 
    
    //Loop through all the files in the list
    while (curr!=NULL) {
        FILE *current = fopen(curr->value, "r");
        
        //count the number of words in the file
        int d = 0;
        int flag = 0;
        char ch=fgetc(current); 
        while(ch!=EOF) { 
            if(ch==' '||ch=='\n') { 
                if (flag==1) flag=0, d++; //if the previous character wasnt space, add a word
            } else {
                flag = 1; //set flag the previous character was a non-space
            }
            ch=fgetc(current); 
        }
        
        //move back to start of file
        rewind(current);
        
         //word;
         //normWord;
        while (fgets(line,1000,current)!=NULL) {
            char *word = strtok(line," \n");
            while (word != NULL && strcmp(word,"\n")!=0) {
                char *normWord = normaliseWord(word);
                root=InvertedIndexBSTInsert(root, normWord, d, curr->value); //Insert the word into the tree
                word = strtok(NULL," \n");
            }
        }
        fclose(current);
        curr=curr->next;
    }
    freeList(filesList); //Free the memory associated with the list of files
    return root; //Return the root of the Inverted Index Tree
}



void printInvertedIndex(InvertedIndexBST tree) {\
    //Open the file for writing, and print the tree in Infix order, then close the file
    FILE *fp = fopen("invertedIndex.txt", "w");
    InvertedIndexBSTreeInfix(tree, fp);
    fclose(fp);
    return;
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D) {
    //Loop through all the files in the file list of the node for the word in the Inverted Index Tree,
    //Calculate their Tf-Idf sum based on the length of the file list for that word, the previously
    //calculated Tf and the provided D, then insert that as a node into a TfIdf list
    TfIdfList root = newTfIdfList();
    InvertedIndexBST word = FindNode(tree, searchWord);
    FileList list = word->fileList;
    int count= LengthFileList(list);
    while (list!=NULL) {
        double tfIdf=calculateTfIdfVal(list->tf, count, D);
        root=TfIdfInsert(root, tfIdf, list->filename);
        list=list->next;
    }
    return root;
}

TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D) {
    //Same as previous function, except looping through multiple search words and
    //either appending them to the tfidf list, or summing together the 2 tfidf values
    //for the file. This causes the files to become out of descending order so the files
    //are reordered into a new TfIdf list and the memory associated with the old list is 
    //freed and the new, ordered list is returned
    int i = 0;
    TfIdfList root = newTfIdfList();
    while (1) {
        if (searchWords[i]==NULL) break;
        InvertedIndexBST word = FindNode(tree, searchWords[i]);
        FileList list = word->fileList;
        int count= LengthFileList(list);
        while (list!=NULL) {
            if (inTfIdfList(root, list->filename)==1) {
                TfIdfList node = findNodeTfIdfList(root, list->filename);
                double tfIdf=calculateTfIdfVal(list->tf, count, D);
                node->tfidf_sum+=tfIdf;
            } else {
                double tfIdf=calculateTfIdfVal(list->tf, count, D);
                root=TfIdfInsert(root, tfIdf, list->filename);
            }
            list=list->next;
        }

        i++;
    }

    TfIdfList ordered = newTfIdfList();
    ordered = reorderTfIdfList(root, ordered);
    free(root);
    return ordered;
}
