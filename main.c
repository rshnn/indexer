#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include <limits.h>
#include <errno.h>


SortedListPtr TokenList;




typedef struct fileRecord{
	char* 	filename;
	int 	frequency;
}fileRecord;


typedef struct Token{
	char*			word;
	SortedListPtr	fileList;
}Token;








char* copyFileName(char* filename)
{
	char *s;
	size_t len = strlen(filename)+1;

	if(len > PATH_MAX){
		fprintf(stderr, "The pathname for %s exceeds the maximum allowed.\n",filename);
		exit(-1);
	}

	if((s = malloc(sizeof(char)*len)) == NULL){
		fprintf(stderr, "errno");
		return NULL;
	}
	else{
		sprintf(s, "%s", filename);
		return s;
	}

}
















void destroyFileRecord(void *pointer)
{
	if (pointer != NULL) {
    	fileRecord *r = pointer;
    	free(r);
  	}
}

void destroyTokenStruct(void *pointer)
{
	Token *tok = pointer;
	free(tok->word);
	free(tok->fileList);
	//SLDestroy(tok->fileList);
	free(tok);
}




/* Comparator function for pointers to stings. */
int compareStrings(void *p1, void *p2)
{
	char *s1 = (char*)p1;
	char *s2 = (char*)p2;

	return strcmp(s1, s2);
}

/* Comparator function for pointers to Token structures. */
int compareTokenStructs(void *p1, void *p2)
{
	struct Token *t1 = (struct Token*)p1;
	struct Token *t2 = (struct Token*)p2;

	char* s1		= (char *)t1->word;
	char* s2 		= (char *)t2->word;

	return strcmp(s2, s1);
}

/* Comparator function for pointers to fileRecord structures. */
int compareRecordStructs(void *p1, void *p2)
{
	struct fileRecord *t1 = (struct fileRecord*)p1;
	struct fileRecord *t2 = (struct fileRecord*)p2;

	int i1 = t1->frequency;
	int i2 = t2->frequency;

	if((i1-i2)==0){
		return compareStrings(t2->filename,t1->filename);
	}else
		return i1-i2;
}


/* Comparator function for pointers to fileRecord structures. */
int compareRecordName(void *p1, void *p2)
{
	struct fileRecord *t1 = (struct fileRecord*)p1;
	struct fileRecord *t2 = (struct fileRecord*)p2;

	return compareStrings(t2->filename,t1->filename);
}














void writeFile(char *filename)
{
  FILE *file;
  if ((file = fopen(filename, "w+")) != NULL) {
    fprintf(file, "{\"list\" : [\n");
    SortedListIteratorPtr itr1 = SLCreateIterator(TokenList);
    Token *tmp = SLGetItem(itr1);

    while (tmp != NULL) {
      fprintf(file, "\t  {\"%s\" : [ \n", tmp->word);
      SortedListIteratorPtr itr2 = SLCreateIterator(tmp->fileList);
      fileRecord *tmp2 = SLGetItem(itr2);

      while (tmp2 != NULL) {

		printf("~~~~~~~~~~~~~~~~%s\n",tmp2->filename);

        fprintf(file, "\t\t  {\"%s\" : %d}", tmp2->filename, tmp2->frequency);
        if (!SLIsLastItem(itr2))
          fprintf(file, ",");
        fprintf(file, "\n");
        tmp2 = SLNextItem(itr2);
      }

      fprintf(file, "\t  ]}");

      if (!SLIsLastItem(itr1))
        fprintf(file, ",");

      fprintf(file, "\n");

      SLDestroyIterator(itr2);
      tmp = SLNextItem(itr1);
    }

    fprintf(file, "]}\n");
    SLDestroyIterator(itr1);
    fclose(file);
  }

  else {
    fprintf(stderr, "%s\n", filename);
    fprintf(stderr, "errno %d: %s\n",errno, strerror(errno));
  }
}
















void processToken(char* word, char* filename)
{
	Token 		*searchToken;
	fileRecord 	*searchRecord;
	Token   	*newToken 	= malloc(sizeof(Token));
	fileRecord  *newRecord 	= malloc(sizeof(fileRecord));
	
	if (newRecord == NULL || newToken == NULL) {
		fprintf(stderr, "errno %d\n", errno);
		fprintf(stderr, "mesg: %s\n", strerror(errno));
		exit(-1);
	}

	newToken->word    	 	= word;
	newToken->fileList   	= NULL;
	newRecord->filename 	= copyFileName(filename);
	newRecord->frequency	= 1;

	if( (searchToken = SLSearch(TokenList, newToken, compareRecordName)) != NULL ){

		if( (searchRecord = SLSearch(searchToken->fileList, newRecord, compareRecordName)) != NULL ){
			printf("~~~~~~Found this one: %s\n", searchRecord->filename);
			searchRecord->frequency++;
			destroyFileRecord(newRecord);
		}
		else{
			SLInsert(searchToken->fileList, newRecord);
		}
		destroyTokenStruct(newToken);
	}
	else{
		newToken->fileList = SLCreate(compareRecordStructs, destroyFileRecord);
		SLInsert(newToken->fileList, newRecord);
		SLInsert(TokenList, newToken);
	}



	// SortedListIteratorPtr testing = SLCreateIterator(TokenList);
	// Token* t = SLGetItem(testing);

	// SortedListIteratorPtr test2 = SLCreateIterator(t->fileList);
	// printf("\n\n%s Test '%s'\n",t->word, ((fileRecord*) SLGetItem(test2))->filename);
	// if(SLNextItem(test2) != NULL)
	// 	printf("\n\n%s Test '%s'\n",t->word, ((fileRecord*) SLGetItem(test2))->filename);

}


/* 
	Creates and parses through an input file using a tokenizer object.
		Caller is responsible for opening and closing the input file.
		Tokenizer object is intialized and destroyed by this function.
*/
int parseFile(FILE* file, char* inputname)
{

	TokenizerT* tk 	= TKCreate(file);

	/* Currently only prints out file contents.  To be changed later */
	char* token;

	token = TKGetNextToken(tk);
	do{

		printf("\tCurrent token is: %s in %s \n",token, inputname);
		if(token == NULL){
			break;
		}

		processToken(token, inputname);

	}while((token = TKGetNextToken(tk)) != 0);


	TKDestroy(tk);
	return 0;
}

/*
	exploreDirectories travels through directories given a char* inputname.
		Recursive calls are initiated on file pointers until Files are reached.
		The base case handles files by calling parseFile.
*/
int exploreDirectories(char* inputname)
{

	/* enum value will keep record of current type */
	enum{File, Directory} type 		= Directory;
	
	void *fileptr					= opendir(inputname);
	struct dirent *entry;

	if(fileptr == NULL){
		type 		= File;
		fileptr 	= fopen(inputname, "r+");
		
		if(fileptr == NULL){
			perror(fileptr);
			return 0;
		}
	}

	/* Current recursive iteration is a directory */
	if(type == Directory){

		DIR *directory 	= (DIR *)fileptr;
		for(;;){
			entry 	= readdir(directory);

			/* Empty directory */
			if(entry == NULL){
				break;
			}

			/* Protection against recursing backwards */
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
				continue;
			}

			/* Save filepath as string */
			char* next 	= (char *)malloc(strlen(inputname) + strlen(entry->d_name) + 2);
			strcpy(next, inputname);
			strcat(next, "/");
			strcat(next, entry->d_name);

			/* Recursive call */
			printf("Current directory:\t%s\n",next);
			exploreDirectories(next);
			free(next);
		}
		closedir(directory);

	}
	/* Current recursive iteration is a file (Base case) */
	else{
		printf(">>Exploring file:\t%s \n", inputname);
		FILE *file = (FILE *)fileptr;
		parseFile(file, inputname);
		printf("\n\n");
		fclose(file);
	}

	return 0;
}














































/* 
	Agument validity checking.  Includes -help option.
	Returns error message on invalid input.
*/
void argCheck(int argc, char** argv)
{

	char* argNumErr = "Incorrect number of arguments given.  Use \"index -help\" for proper usage infomation.\n";  
	switch(argc){
		case 2:
			if ( strcmp(argv[1], "-help") == 0 ){
				fprintf(stderr, "\nProper usage:  ./index <inverted-index file name> <directory or file name>\n");
			}
			exit(0);

		case 3:
			break;
	
		default: 
			fprintf(stderr, "%s\n", argNumErr);
			exit(0);
	}
}


/* 
	TESTING FUNCTIONALITY OF TOKENIZER.
	SHOULD SPIT OUT INPUT FILE'S ALPHANUM CHARS IN LOWERCASE 
*/
int main(int argc, char **argv)
{

	TokenList = SLCreate(compareTokenStructs, destroyTokenStruct);

	argCheck(argc, argv);
	exploreDirectories(argv[2]);


	writeFile("output1.txt");


	SortedListIteratorPtr testing = SLCreateIterator(TokenList);
	Token* t = SLGetItem(testing);

	SortedListIteratorPtr test2 = SLCreateIterator(t->fileList);
	printf("\n\n%s Yes'%s'\n",t->word, ((fileRecord*) SLGetItem(test2))->filename);


	return 0;
}
